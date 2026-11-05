#include "main.h"

/**
 * _strncpy - Copies a string up to n bytes.
 * @dest: The destination buffer to copy to.
 * @src: The source string to copy from.
 * @n: The maximum number of bytes to write.
 *
 * Return: A pointer to the destination string dest.
 */
char *_strncpy(char *dest, char *src, int n)
{
	int i = 0;

	/* Copy characters from src to dest up to n bytes */
	while (i < n && src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}

	/* Pad the remainder of dest with null bytes if src was shorter than n */
	while (i < n)
	{
		dest[i] = '\0';
		i++;
	}

	return (dest);
}
