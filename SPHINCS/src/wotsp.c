/*
 * wotsp.c
 *
 */

#include "wotsp.h"
#include "hash.h"
#include "prng.h"

int wotsp_keygen(unsigned char y[WOTS_L*SPHINCS_BYTES],
                 unsigned char x[WOTS_L*SPHINCS_BYTES],
                 unsigned char const sk1[SEED_BYTES],
                 unsigned char const masks[(WOTS_MAX_INT-1)*SPHINCS_BYTES])
{
	int i;

	/* Recovers secret key with PRNG  */
	prng(x, WOTS_L*SPHINCS_BYTES, sk1);

	/* Chains hash functions WOTS_L times */
	for (i = 0; i < WOTS_L; ++i)
	{
		hash_chain_n_mask(y + i*SPHINCS_BYTES, x + i*SPHINCS_BYTES,
		                  masks, WOTS_MAX_INT - 1);
	}

	return 0;
}

int wotsp_sign(unsigned char const message[SPHINCS_BYTES],
               unsigned char const sk[SEED_BYTES],
               unsigned char const masks[(WOTS_MAX_INT-1)*SPHINCS_BYTES])
{
	return 1; /* true */
}

int wotsp_verify(unsigned char pk[WOTS_L*SPHINCS_BYTES],
                 unsigned char const sig[WOTS_L*SPHINCS_BYTES],
                 unsigned char const message[SPHINCS_BYTES],
                 unsigned char const masks[(WOTS_MAX_INT-1)*SPHINCS_BYTES])
{
	return 1; /* true */
}
