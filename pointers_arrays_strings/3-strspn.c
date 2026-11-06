#include "main.h"

/**
 * _strspn - Gets the length of a prefix substring.
 * @s: The initial segment string to examine.
 * @accept: The string containing the list of characters to match.
 *
 * Return: The number of bytes in the initial segment of @s
 *         which consist entirely of bytes from @accept.
 */
unsigned int _strspn(char *s, char *accept)
{
	unsigned int count = 0;
	int i;
	int match;

	while (*s)
	{
		match = 0;
		for (i = 0; accept[i] != '\0'; i++)
		{
			if (*s == accept[i])
			{
				match = 1;
				break;
			}
		}

		if (match == 0)
		{
			return (count);
		}

		count++;
		s++;
	}

	return (count);
}
