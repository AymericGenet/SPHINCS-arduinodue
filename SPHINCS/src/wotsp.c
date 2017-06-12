/*
 * wotsp.c
 *
 */

#include <stdio.h>
#include "wotsp.h"
#include "hash.h"
#include "prng.h"

#define SPLIT_4(i, b, digest, checksum) do { \
	if (i < WOTS_L1) { \
		if ((i & 1) == 0) { \
			b = ((digest[i/2] >> WOTS_W) & 0x0f); \
		} else { \
			b = (digest[i/2] & 0x0f); \
		} \
		checksum += (WOTS_MAX_INT - b); \
	} else { \
		b = ((checksum >> (WOTS_W*(WOTS_L - i - 1))) & 0x0f); \
	} \
} while (0)

int wotsp_keygen(unsigned char y[WOTS_L*SPHINCS_BYTES],
                 unsigned char const seed[SEED_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES])
{
	int i, j;

	/* Recovers secret key with PRNG */
	prng(y, WOTS_L*SPHINCS_BYTES, seed);

	/* Chains hash functions WOTS_L times */
	for (i = 0; i < WOTS_L; ++i)
	{
		hash_chain_n_mask(y + i*SPHINCS_BYTES, y + i*SPHINCS_BYTES, masks, WOTS_MAX_INT);
	}

	return 0;
}

int wotsp_sign(unsigned char const digest[SPHINCS_BYTES],
               unsigned char const seed[SEED_BYTES],
               unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES])
{
	int i, j;
	unsigned long b, checksum;
	unsigned char x[WOTS_L*SPHINCS_BYTES];

	/* Recovers secret key with PRNG */
	prng(x, WOTS_L*SPHINCS_BYTES, seed);

	for (i = 0, checksum = 0; i < WOTS_L; ++i)
	{
		SPLIT_4(i, b, digest, checksum);

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
	unsigned long b, checksum;
	unsigned char tmp[SPHINCS_BYTES];

	for (i = 0, checksum = 0; i < WOTS_L; ++i)
	{
		SPLIT_4(i, b, digest, checksum);

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
