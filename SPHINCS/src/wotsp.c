/*
 * wotsp.c
 *
 */

#include <stdio.h>
#include "wotsp.h"
#include "hash.h"
#include "prng.h"

static int split_4(unsigned long long out[WOTS_L],
                   unsigned char const in[SPHINCS_BYTES])
{
	int i, c = 0;

	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		out[2*i] = (in[i] >> WOTS_W) & 0x0f;
		c += WOTS_MAX_INT - out[2*i];

		out[(2*i)+1] = in[i] & 0x0f;
		c += WOTS_MAX_INT - out[(2*i)+1];
	}

	for (i = 0; i < WOTS_L2; ++i)
	{
		out[WOTS_L1 + i] = c & 0x0f;
		c = (c >> WOTS_W);
	}

	return 0;
}

static unsigned long next_split_4(int idx,
                                  unsigned char const in[SPHINCS_BYTES])
{
	int i, c = 0;

	if (idx < WOTS_L1)
	{
		for (i = 0; i < SPHINCS_BYTES; ++i)
		{
			if (idx == 2*i)
			{
				return ((in[i] >> WOTS_W) & 0x0f);
			}

			if (idx == 2*i + 1)
			{
				return (in[i] & 0x0f);
			}
		}
	}
	else
	{
		for (i = 0; i < SPHINCS_BYTES; ++i)
		{
			c += WOTS_MAX_INT - ((in[i] >> WOTS_W) & 0x0f);
			c += WOTS_MAX_INT - (in[i] & 0x0f);
		}

		for (i = 0; i < WOTS_L2; ++i)
		{
			if (i + WOTS_L1 == idx)
			{
				return (c & 0x0f);
			}
			c = (c >> WOTS_W);
		}
	}

	return 0;
}

int wotsp_keygen(unsigned char const sk1[SEED_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES])
{
	int i, j;
	unsigned char x[WOTS_L*SPHINCS_BYTES];

	/* Recovers secret key with PRNG */
	prng(x, WOTS_L*SPHINCS_BYTES, sk1);

	/* Chains hash functions WOTS_L times */
	for (i = 0; i < WOTS_L; ++i)
	{
		hash_chain_n_mask(x + i*SPHINCS_BYTES, x + i*SPHINCS_BYTES, masks, WOTS_MAX_INT);

		/* Outputs on the stdout */
		for (j = 0; j < SPHINCS_BYTES; ++j)
		{
			printf("%02x", x[i*SPHINCS_BYTES + j]);
		}
	}

	return 0;
}

int wotsp_sign(unsigned char const digest[SPHINCS_BYTES],
               unsigned char const sk1[SEED_BYTES],
               unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES])
{
	int i, j;
	unsigned long long b;
	unsigned char x[WOTS_L*SPHINCS_BYTES];

	/* Recovers secret key with PRNG */
	prng(x, WOTS_L*SPHINCS_BYTES, sk1);

	for (i = 0; i < WOTS_L; ++i)
	{
		b = next_split_4(i, digest);
		hash_chain_n_mask(x + i*SPHINCS_BYTES, x + i*SPHINCS_BYTES, masks, b);

		/* Outputs on the stdout */
		for (j = 0; j < SPHINCS_BYTES; ++j)
		{
			printf("%02x", x[i*SPHINCS_BYTES + j]);
		}
	}

	return 0;
}

int wotsp_verify(unsigned char const digest[SPHINCS_BYTES],
                 unsigned char const y[WOTS_L*SPHINCS_BYTES],
                 unsigned char const sig[WOTS_L*SPHINCS_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES])
{
	int i, j;
	unsigned long long b;
	unsigned char tmp[SPHINCS_BYTES];

	for (i = 0; i < WOTS_L; ++i)
	{
		b = next_split_4(i, digest);
		hash_chain_n_mask(tmp, sig + i*SPHINCS_BYTES,
		                  masks + b*SPHINCS_BYTES,
		                  WOTS_MAX_INT - b);

		/* Verifies that (tmp == y) */
		for (j = 0; j < SPHINCS_BYTES; ++j)
		{
			if (tmp[j] != y[j + i*SPHINCS_BYTES])
			{
				return 0; /* False */
			}
		}
	}

	return 1; /* True */
}
