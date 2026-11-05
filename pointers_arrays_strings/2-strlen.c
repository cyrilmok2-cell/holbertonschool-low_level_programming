#include "main.h"

/**
 * _strlen - Returns the length of a string.
 * @s: A pointer to the character string to check.
 *
 * Return: The length of the string as an integer.
 */
int _strlen(char *s)
{
	int length;

	length = 0;
	while (s[length] != '\0')
	{
		length++;
	}

	return (length);
}
