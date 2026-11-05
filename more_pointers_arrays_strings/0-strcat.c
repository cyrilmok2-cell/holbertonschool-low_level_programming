#include "main.h"

/**
 * _strcat - Concatenates two strings by appending src to dest.
 * @dest: The destination string to be appended to.
 * @src: The source string to append.
 *
 * Return: A pointer to the resulting string dest.
 */
char *_strcat(char *dest, char *src)
{
	int dest_len = 0;
	int i = 0;

	/* Move to the end of dest string */
	while (dest[dest_len] != '\0')
	{
		dest_len++;
	}

	/* Copy src onto the end of dest */
	while (src[i] != '\0')
	{
		dest[dest_len] = src[i];
		dest_len++;
		i++;
	}

	/* Append the closing null terminator */
	dest[dest_len] = '\0';

	return (dest);
}
