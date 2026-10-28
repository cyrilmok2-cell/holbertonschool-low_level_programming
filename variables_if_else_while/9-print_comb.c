#include <stdio.h>

/**
 * main - prints all possible combinations of single-digit numbers
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	int n;

	for (n = 48; n <= 57; n++)
	{
		putchar(n); /* 1st putchar: Prints the number (ASCII 48 is '0') */
		if (n != 57) /* If it's not '9' (ASCII 57) */
		{
			putchar(','); /* 2nd putchar: Prints the comma */
			putchar(' '); /* 3rd putchar: Prints the space */
		}
	}
	putchar('\n'); /* 4th putchar: Prints the final newline */

	return (0);
}
