/*
 * horst.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "horst.h"
#include "trees.h"
#include "parameters.h"
#include "prng.h"
#include "hash.h"
#include "ecrypt-sync.h"

#define SPLIT_16(i, m, digest) do { \
	m = digest[2*i]; /* unsure about long promotion */ \
	m = (m << 8) + digest[2*i + 1]; \
} while (0)

static ECRYPT_ctx ctx;
static unsigned char tmp[2*SPHINCS_BYTES];

static int horst_leafcalc(struct Node * node, unsigned int leaf)
{
	if (leaf >= HORST_T)
	{
		printf("Failed?\n");
		return 0; /* False */
	}
	else {
		if ((leaf & 1) == 0)
		{
			/* Recover 2 secret values */
			prng_next(&ctx, tmp);
			/* Hash them */
			hash_n_n(tmp, tmp);
			hash_n_n(tmp + SPHINCS_BYTES, tmp + SPHINCS_BYTES);
		}
		memcpy(node->hash, tmp + (leaf & 1)*SPHINCS_BYTES, SPHINCS_BYTES);
		node->level = 0;
		node->id = leaf;
		return 1; /* True */
	}
}

int horst_keygen(unsigned char const seed[SEED_BYTES],
                 unsigned char const masks[HORST_TAU*SPHINCS_BYTES])
{
	int i, j;
	struct Node node;
	struct Stack stack;
	stack.index = -1;

	/* Sets up context */
	prng_context_setup(&ctx, seed);

	/* Constructs the Merkle tree root with treehash */
	horst_leafcalc(&node, 0);
	l_treehash_mask(&node, &stack, 2*(HORST_T - 1), 1, horst_leafcalc, masks);

	/* Deletes the context */
	prng_context_delete(&ctx);

	/* Outputs on the stdout */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		printf("%02x", node.hash[i]);
	}

	return 0;
}

int horst_sign(unsigned char const digest[SPHINCS_BYTES],
               unsigned char const seed[SEED_BYTES],
               unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES])
{
	int i, j;
	unsigned char x[HORST_T*SPHINCS_BYTES];
	unsigned int m[HORST_K];

	/* Splits digest in HORST_K chunks of HORST_TAU bytes */
	for (i = 0; i < HORST_K; ++i)
	{
		SPLIT_16(i, m[i], digest);
	}

	

	/* Outputs on the stdout */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		printf("%02x", x[i]);
	}

	return 0;
}

int horst_verify(unsigned char const digest[SPHINCS_BYTES],
                 unsigned char const pk[WOTS_L*SPHINCS_BYTES],
                 unsigned char const sig[WOTS_L*SPHINCS_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES])
{
	
}
