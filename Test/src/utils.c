/*
 * utils.c
 *
 */

#include "utils.h"

int array_cmp(unsigned char const * arr1, unsigned char const * arr2, int len)
{
	int i;

	for (i = 0; i < len; ++i)
	{
		if (arr1[i] != arr2[i])
		{
			return 0; /* False */
		}
	}
	return 1; /* True */
}
