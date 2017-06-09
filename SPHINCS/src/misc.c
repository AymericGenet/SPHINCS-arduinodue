/*
 * misc.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "misc.h"
#include "trees.h"
#include "parameters.h"

int uint_cmp(const void * a, const void * b)
{
	return (*(unsigned int *)a) < (*(unsigned int *)b) ? -1 : 1 /* true */;
}

int parse_sk(unsigned char * sk_hash, int (*sig_stream)())
{
	int ch = 0, i = 0;
	unsigned char string[2*SPHINCS_BYTES];
	unsigned char hex[2];
	unsigned long tmp = 0;

	/* Skip the parentheses */
	while ((ch = sig_stream()) != ')')
	{
		if (ch == EOF)
		{
			return 0; /* False */
		}
	}

	/* Store hexstring until the end of line is reached */
	while ((ch = sig_stream()) != EOF && ch != '\n')
	{
		if ((ch >= '0' && ch <= '9') ||
		    (ch >= 'a' && ch <= 'z') ||
		    (ch >= 'A' && ch <= 'Z'))
		{
			/* If length is incorrect, abort */
			if (i >= 2*SPHINCS_BYTES)
			{
				return 0; /* False */
			}
			string[i] = ch;
			i++;
		}
	}

	/* If length is incorrect, abort */
	if (i != 2*SPHINCS_BYTES)
	{
		return 0; /* False */
	}

	/* Parse the hexstring */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		hex[0] = string[2*i];
		hex[1] = string[2*i + 1];
		tmp = strtoul(hex, NULL, 16);
		sk_hash[i] = (unsigned char) tmp;
	}

	return 1; /* True */
}

int parse_node(struct Node * node, int (*sig_stream)())
{
	int ch = 0, i = 0;
	unsigned char string[2*SPHINCS_BYTES + 1];
	unsigned char hex[2];
	unsigned long tmp = 0;

	string[2*SPHINCS_BYTES] = '\0';

	/* Skip until height is reached */
	while ((ch = sig_stream()) != 'h')
	{
		if (ch == EOF)
		{
			return 0; /* False */
		}
	}
	if ((ch = sig_stream()) != '=')
	{
		return 0; /* False */
	}

	/* Parse height digits */
	while ((ch = sig_stream()) != ',')
	{
		if ((ch >= '0' && ch <= '9'))
		{
			/* If length is incorrect, abort */
			if (i >= 2*SPHINCS_BYTES)
			{
				return 0; /* False */
			}
			string[i] = ch;
			i++;
		}
		else
		{
			return 0; /* False */
		}
	}

	/* Parse digit string in node level */
	string[i] = '\0';
	tmp = strtoul(string, NULL, 10);
	node->level = (unsigned int) tmp;

	/* Skip until id is reached */
	while ((ch = sig_stream()) != 'i')
	{
		if (ch == EOF)
		{
			return 0; /* False */
		}
	}
	if ((ch = sig_stream()) != 'd' || (ch = sig_stream()) != '=')
	{
		return 0; /* False */
	}

	/* Store id digits */
	i = 0;
	while ((ch = sig_stream()) != ')')
	{
		if ((ch >= '0' && ch <= '9'))
		{
			/* If length is incorrect, abort */
			if (i >= 2*SPHINCS_BYTES)
			{
				return 0; /* False */
			}
			string[i] = ch;
			i++;
		}
		else
		{
			return 0; /* False */
		}
	}

	/* Parse digit string in node id */
	string[i] = '\0';
	tmp = strtoul(string, NULL, 10);
	node->id = (unsigned int) tmp;

	/* Store hexstring until the end of line is reached */
	i = 0;
	while ((ch = sig_stream()) != EOF && ch != '\n')
	{
		if ((ch >= '0' && ch <= '9') ||
		    (ch >= 'a' && ch <= 'z') ||
		    (ch >= 'A' && ch <= 'Z'))
		{
			/* If length is incorrect, abort */
			if (i >= 2*SPHINCS_BYTES)
			{
				return 0; /* False */
			}
			string[i] = ch;
			i++;
		}
	}

	/* If length is incorrect, abort */
	if (i != 2*SPHINCS_BYTES)
	{
		return 0; /* False */
	}

	/* Parse the hexstring */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		hex[0] = string[2*i];
		hex[1] = string[2*i + 1];
		tmp = strtoul(hex, NULL, 16);
		node->hash[i] = (unsigned char) tmp;
	}

	return 1; /* True */
}
