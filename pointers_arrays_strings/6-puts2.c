#include "main.h"

/**
 * puts2 - Prints every other character of a string,
 * starting with the first character, followed by a new line.
 * @str: The string to be processed and printed.
 *
 * Return: void
 */
void puts2(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		_putchar(str[i]);
		i += 2;
		if (str[i - 1] == '\0')
		{
			break;
		}
	}
	_putchar('\n');
}
