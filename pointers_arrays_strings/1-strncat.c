#include "main.h"

/**
 * _strncat - Concatenates two strings using at most n bytes from src.
 * @dest: The destination string to append to.
 * @src: The source string to append.
 * @n: The maximum number of bytes to copy from src.
 *
 * Return: A pointer to the resulting string dest.
 */
char *_strncat(char *dest, char *src, int n)
{
	int dest_len = 0;
	int i = 0;

	/* Find the end of dest string */
	while (dest[dest_len] != '\0')
	{
		dest_len++;
	}

	/* Copy at most n bytes from src to dest */
	while (i < n && src[i] != '\0')
	{
		dest[dest_len] = src[i];
		dest_len++;
		i++;
	}

	/* Always terminate string cleanly with null byte */
	dest[dest_len] = '\0';

	return (dest);
}
