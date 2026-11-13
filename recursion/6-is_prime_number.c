#include "main.h"

/**
 * _prime_helper - checks recursively if a number is divisible by i
 * @n: the number to check
 * @i: the current divisor to test
 *
 * Return: 1 if prime, 0 otherwise
 */
int _prime_helper(int n, int i)
{
	if (n % i == 0)
		return (0);
	if (i * i > n)
		return (1);
	return (_prime_helper(n, i + 1));
}

/**
 * is_prime_number - returns 1 if the input integer is a prime number
 * @n: the number to evaluate
 *
 * Return: 1 if prime, 0 otherwise
 */
int is_prime_number(int n)
{
	if (n <= 1)
		return (0);
	return (_prime_helper(n, 2));
}
