# Memory Maps and Pointer Lifetime Analysis

## 1. `stack_example.c` — Recursion and Stack Frames

### Memory Region Classification
* **Stack:** Every variable in `stack_example.c` resides exclusively on the call stack:
  * `walk_stack`: parameters `depth` (`int`), `max_depth` (`int`), and local variable `marker` (`int`, initialized to `depth * 10`).
  * `dump_frame`: parameters `label` (`const char *`), `depth` (`int`), and local variables `local_int` (`int`), `local_buf` (`char[16]`), and `p_local` (`int *`, holding `&local_int`).
* **Heap:** None (`0` bytes allocated on the heap; no calls to `malloc`/`calloc`/`realloc`).
* **Read-Only Data (`.rodata`):** String literals such as `"enter"`, `"exit"`, and `"stack_example: recursion and stack frames\n"`.

### Step-by-Step Execution & Stack Frame Lifecycle
1. **`main()` entry:** Pushes `main` frame onto the stack and calls `walk_stack(0, 3)`.
2. **Recursive Descent (`depth = 0 -> 1 -> 2 -> 3`):**
   * At each level, a new `walk_stack` stack frame is pushed at a **lower memory address** (stack grows downward on x86_64). Each frame stores its own independent `marker` (`0`, `10`, `20`, `30`).
   * Inside each `walk_stack` call, `dump_frame("enter", depth)` pushes a temporary `dump_frame` frame below the current `walk_stack` frame. Inside `dump_frame`, `p_local` aliases `local_int` (`p_local == &local_int`).
   * As soon as `dump_frame("enter", depth)` returns, its stack frame is **popped** and its local variables (`local_int`, `local_buf`, `p_local`) reach the end of their lifetime.
   * When `walk_stack(depth + 1, 3)` is subsequently called, the new `walk_stack` frame reuses the stack space previously occupied by the popped `dump_frame("enter", depth)` frame.
3. **Base Case (`depth == 3`):** `if (depth < max_depth)` evaluates to false, stopping recursion. `dump_frame("exit", 3)` runs and pops, then `walk_stack(3, 3)` returns and pops.
4. **Recursive Unwinding (`depth = 2 -> 1 -> 0`):**
   * Each suspended `walk_stack` frame resumes at its exact saved `marker` address and value (`20`, `10`, `0`), calls `dump_frame("exit", depth)`, and then pops its own frame upon return.
   * Notice that `dump_frame("enter", d)` and `dump_frame("exit", d)` at the same recursion depth `d` share identical stack addresses for `&local_int` and `local_buf` because the stack pointer (`%rsp`) returns to the exact same offset before each call.

### Documented AI Audit & Correction (`stack_example.c`)
* **AI Claim (Incorrect):** An AI visualizer initially claimed that *"all four `dump_frame` calls during recursive descent (`depth = 0, 1, 2, 3`) remain active on the stack simultaneously, each occupying a distinct non-overlapping region below all `walk_stack` frames, and that `p_local` remains valid until `main()` exits."*
* **Why It Is Wrong:**
  1. `dump_frame` is **not** recursive—it returns before `walk_stack` recurses (`walk_stack(depth + 1, max_depth)`). Therefore, `dump_frame`'s stack frame is popped immediately after `dump_frame("enter", depth)` finishes, and subsequent recursive calls overwrite that reclaimed stack memory.
  2. `p_local` is a stack-allocated pointer pointing to `local_int` in the same frame; both die as soon as `dump_frame` returns.
* **Correction:** Only the active chain of `walk_stack` frames (`depth 0` through current `depth`) plus at most **one** active `dump_frame` frame exist on the stack at any instant. Lifetime of `local_int`, `local_buf`, and `p_local` is strictly bounded to a single invocation of `dump_frame`.

---

## 2. `aliasing_example.c` — Heap Allocation, Aliasing, and Use-After-Free

### Memory Region Classification
* **Stack:**
  * `main`: local pointer `a` (`int *`), local pointer `b` (`int *`), local integer `n` (`int = 5`).
  * `make_numbers`: parameter `n` (`int`), local loop counter `i` (`int`), local pointer `arr` (`int *`).
* **Heap:**
  * Inside `make_numbers(5)`, `malloc(5 * sizeof(int))` allocates a contiguous **20-byte block** on the heap (`5 * 4` bytes) initialized to `[0, 11, 22, 33, 44]`.

### Step-by-Step Execution & Pointer Aliasing
1. **`a = make_numbers(n);`:**
   * `make_numbers` allocates the 20-byte heap block and stores its base address in stack variable `arr`.
   * On return, `make_numbers`'s stack frame (`n`, `i`, `arr`) is popped, while the **20-byte heap block persists** because heap lifetime is manual. Its base address is copied into `main`'s stack variable `a`.
2. **`b = a;` (Pointer Aliasing):**
   * No new heap allocation occurs. Stack variable `b` receives the exact same heap address stored in `a` (`a == b`, while `&a != &b` on the stack).
   * `a` and `b` are now **aliases** pointing to the same 20-byte heap object (`a[2]` and `b[2]` both read `22` from address `(char *)a + 8`).
3. **`free(a);` (Object Lifetime Ends, Dangling Alias Remains):**
   * `free(a)` releases the 20-byte heap block back to the heap allocator (`glibc` `ptmalloc`).
   * Neither `a` nor `b` is automatically set to `NULL`. Both `a` and `b` still hold the old heap address, making **`b` (and `a`) a dangling pointer**.
4. **`printf(" reading b[2]=%d\n", b[2]);` and `b[3] = 1234;` (Use-After-Free):**
   * Dereferencing `b` and writing `1234` to `b[3]` accesses freed heap memory (**Undefined Behavior / Use-After-Free**).
   * When run without Valgrind, the program often appears to "succeed" and prints `b[2]=22` and `b[3]=1234` because the allocator has not yet unmapped the page or overwritten those bytes (though `ptmalloc` metadata in the first 16 bytes `b[0..3]` may partially overlap freed chunk pointers).

### Documented AI Audit & Correction (`aliasing_example.c`)
* **AI Claim (Incorrect):** An AI explanation claimed that *"calling `free(a)` only frees pointer `a`, leaving pointer `b` with its own valid copy of the integer array until `main` returns, which is why `b[2]` still prints `22` and `b[3] = 1234` safely updates `b`'s array."*
* **Why It Is Wrong:**
  1. `b = a` copies the **pointer value (memory address)** on the stack, not the 20-byte heap buffer. There is only **one** heap allocation.
  2. `free(a)` deallocates the underlying heap object referenced by that address, immediately terminating the lifetime of the entire 20-byte block for **all** aliases (`a` and `b` alike).
  3. Reading `b[2]` and writing `b[3] = 1234` after `free(a)` is a **Use-After-Free** memory safety bug (C99/C89 undefined behavior). Printing `22` without a crash is a symptom of allocator caching, not valid memory ownership.
* **Correction:** Both `a` and `b` become dangling pointers at `free(a)`. To prevent use-after-free, `b` must not be dereferenced after `free(a)`, and both `a = NULL; b = NULL;` should be set immediately after deallocation.

---

## 3. `heap_example.c` — Nested Heap Allocations and Partial Free (Memory Leak)

### Memory Region Classification
* **Stack:**
  * `main`: `Person *alice`, `Person *bob`.
  * `person_new`: parameters `name` (`const char *`), `age` (`int`), locals `i` (`size_t`), `len` (`size_t`), `p` (`Person *`).
  * `person_free_partial`: parameter `p` (`Person *`).
* **Heap (4 separate allocations total):**
  1. `alice` struct: `malloc(sizeof(Person))` (16 bytes on 64-bit: 8-byte `char *name` + 4-byte `int age` + 4-byte alignment padding).
  2. `alice->name` buffer: `malloc(6)` (`"Alice\0"`, 6 bytes).
  3. `bob` struct: `malloc(sizeof(Person))` (16 bytes).
  4. `bob->name` buffer: `malloc(4)` (`"Bob\0"`, 4 bytes).

### Object Lifetimes & Leak Analysis
* **`bob` (Properly Freed):** `free(bob->name)` frees the 4-byte string buffer first, and `free(bob)` frees the 16-byte `Person` struct second. Zero bytes leaked for `bob`.
* **`alice` (Definite Memory Leak):** `person_free_partial(alice)` calls `free(alice)` *without* first calling `free(alice->name)`.
  * Once the 16-byte `alice` struct is freed, the only pointer to the 6-byte `"Alice\0"` heap block (`alice->name`) is lost.
  * Those **6 bytes remain allocated on the heap until process exit** with no remaining pointer referencing them (**definite memory leak**).

### Documented AI Audit & Correction (`heap_example.c`)
* **AI Claim (Incorrect):** An AI summary stated that *"calling `free(p)` inside `person_free_partial(alice)` automatically frees the `Person` struct along with the `name` string inside it."*
* **Why It Is Wrong:** In C, `free()` is not recursive and does not inspect struct fields. `alice` and `alice->name` were created by **two separate `malloc` calls** at distinct heap addresses. Freeing `alice` only releases the 16-byte struct containing the pointer `name`, orphaning the 6-byte character array on the heap.
* **Correction:** `alice->name` must be freed **before** `free(alice)` (matching the cleanup order used for `bob`), otherwise 6 bytes are definitively leaked.
