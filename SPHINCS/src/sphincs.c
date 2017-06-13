/*
 * sphincs.c
 *
 */

#include "parameters.h"
#include "prf.h"
#include "hash.h"
#include "trees.h"
#include "wotsp.h"
#include <string.h>
#include <stdio.h>

int sphincs_keygen(unsigned char pk[SPHINCS_BYTES],
                   unsigned char const sk1[SEED_BYTES],
                   unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES])
{
	int i = 0, j = 0, l = 0, h = 0;
	unsigned char addr[SPHINCS_ADDRESS_BYTES];
	unsigned char seed[SPHINCS_BYTES];
	unsigned char leaves[MSS_TOTAL_LEAVES*SPHINCS_BYTES];
	unsigned char wots_pk[WOTS_L*SPHINCS_BYTES];

	/* Initializes address to 0 */
	memset(addr, 0, SPHINCS_ADDRESS_BYTES);

	/* Recovers all the leaves from the top subtree */
	for (l = 0; l < MSS_TOTAL_LEAVES; ++l)
	{
		/* Recovers seed at address = (d-1 || 0 || leafidx) */
		addr[SPHINCS_ADDRESS_BYTES - 1] = l;
		addr[0] = ((MSS_LEVELS - 1) << 4);
		prf_64(seed, addr, sk1);

		/* Generates WOTS+ public key and compresses it to the root of an L-tree */
		wotsp_keygen(wots_pk, seed, masks);
		TREE_CONSTRUCTION_MASK(i, j, h, WOTS_L, wots_pk, masks);

		/* Stores WOTS+ compressed key as leaf of top tree */
		for (i = 0; i < SPHINCS_BYTES; ++i)
		{
			leaves[l*SPHINCS_BYTES + i] = wots_pk[i];
		}
	}

	/* Constructs the top Merkle subtree root */
	TREE_CONSTRUCTION_MASK(i, j, h, MSS_TOTAL_LEAVES, leaves, masks);

	/* Copies on the public-key */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		pk[i] = leaves[i];
	}

	return 0;
}
