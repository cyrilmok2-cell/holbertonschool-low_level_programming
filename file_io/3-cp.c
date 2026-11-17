#include "main.h"

/**
 * close_fd - Closes a file descriptor and handles errors.
 * @fd: The file descriptor to close.
 */
void close_fd(int fd)
{
	if (close(fd) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
		exit(100);
	}
}

/**
 * copy_content - Reads from source fd and writes to destination fd.
 * @fd_from: Source file descriptor.
 * @fd_to: Destination file descriptor.
 * @file_from: Name of source file.
 * @file_to: Name of destination file.
 */
void copy_content(int fd_from, int fd_to, char *file_from, char *file_to)
{
	ssize_t r_bytes, w_bytes;
	char buffer[1024];

	while ((r_bytes = read(fd_from, buffer, 1024)) > 0)
	{
		w_bytes = write(fd_to, buffer, r_bytes);
		if (w_bytes == -1 || w_bytes != r_bytes)
		{
			dprintf(STDERR_FILENO, "Error: Can't write to %s\n", file_to);
			close_fd(fd_from);
			close_fd(fd_to);
			exit(99);
		}
	}
	if (r_bytes == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", file_from);
		close_fd(fd_from);
		close_fd(fd_to);
		exit(98);
	}
}

/**
 * main - Copies the content of a file to another file.
 * @argc: Number of command-line arguments.
 * @argv: Array of command-line argument strings.
 *
 * Return: 0 on success.
 */
int main(int argc, char *argv[])
{
	int fd_from, fd_to;

	if (argc != 3)
	{
		dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
		exit(97);
	}
	fd_from = open(argv[1], O_RDONLY);
	if (fd_from == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", argv[1]);
		exit(98);
	}
	fd_to = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0664);
	if (fd_to == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't write to %s\n", argv[2]);
		close_fd(fd_from);
		exit(99);
	}
	copy_content(fd_from, fd_to, argv[1], argv[2]);
	close_fd(fd_from);
	close_fd(fd_to);
	return (0);
}
