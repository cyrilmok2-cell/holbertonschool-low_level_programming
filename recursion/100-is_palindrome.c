#include "main.h"

/**
 * _strlen_rec - returns the length of a string
 * @s: pointer to the string
 *
 * Return: length of the string
 */
int _strlen_rec(char *s)
{
	if (*s == '\0')
		return (0);
	return (1 + _strlen_rec(s + 1));
}

/**
 * _check_pal - checks if a string is a palindrome recursively
 * @s: pointer to the string
 * @left: starting index
 * @right: ending index
 *
 * Return: 1 if palindrome, 0 otherwise
 */
int _check_pal(char *s, int left, int right)
{
	if (left >= right)
		return (1);
	if (s[left] != s[right])
		return (0);
	return (_check_pal(s, left + 1, right - 1));
}

/**
 * is_palindrome - returns 1 if a string is a palindrome and 0 if not
 * @s: pointer to the string to check
 *
 * Return: 1 if palindrome, 0 otherwise
 */
int is_palindrome(char *s)
{
	int len = _strlen_rec(s);

	if (len == 0)
		return (1);
	return (_check_pal(s, 0, len - 1));
}
