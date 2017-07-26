/*
 * sphincs.c
 *
 */

#include "parameters.h"
#include "prf.h"
#include "hash.h"
#include "trees.h"
#include "wotsp.h"
#include "horst.h"
#include <string.h>
#include <stdio.h>

int sphincs_keygen(unsigned char pk[PK_BYTES],
                   unsigned char const sk1[SEED_BYTES],
                   unsigned char const masks[MASKS_BYTES])
{
	int i = 0, j = 0, l = 0, h = 0;
	unsigned char addr[SPHINCS_ADDRESS_BYTES];
	unsigned char seed[SEED_BYTES];
	unsigned char leaves[MSS_TOTAL_LEAVES*SPHINCS_BYTES];
	unsigned char wots_pk[WOTS_L*SPHINCS_BYTES];

	/* Initializes address to 0 */
	memset(addr, 0, SPHINCS_ADDRESS_BYTES);

	/* Recovers all the leaves from the top subtree */
	addr[0] = ((MSS_LEVELS - 1) << 4);
	for (l = 0; l < MSS_TOTAL_LEAVES; ++l)
	{
		/* Recovers seed at address = (d-1 || 0 || leafidx) */
		addr[SPHINCS_ADDRESS_BYTES - 1] = l;
		prf_64(seed, addr, sk1);

		/* Generates WOTS+ public key and compresses it to the root of an L-tree */
		wotsp_keygen(wots_pk, seed, masks);
		TREE_CONSTRUCTION_MASK(i, j, h, WOTS_L, wots_pk, masks);

		/* Stores WOTS+ compressed key as leaf of top tree */
		for (i = 0; i < SPHINCS_BYTES; ++i)
		{
			leaves[l*SPHINCS_BYTES + i] = wots_pk[0*SPHINCS_BYTES + i];
		}
	}

	/* Constructs the top Merkle subtree root */
	TREE_CONSTRUCTION_MASK(i, j, h, MSS_TOTAL_LEAVES, leaves, masks);

	/* Copies on the public-key */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		pk[i] = leaves[0*SPHINCS_BYTES + i];
	}

	return 0;
}

int sphincs_sign(unsigned char const * message,
                 unsigned long long length,
                 unsigned char const sk[SK_BYTES],
                 unsigned char const masks[MASKS_BYTES])
{
	int a = 0, h = 0, i = 0, j = 0, l = 0;
	unsigned char leafidx = 0;
	unsigned char R[2*SPHINCS_BYTES];
	unsigned char digest[SPHINCS_DIGEST_BYTES];
	unsigned char seed[SEED_BYTES];
	unsigned char leaves[MSS_TOTAL_LEAVES*SPHINCS_BYTES];
	unsigned char wots_pk[WOTS_L*SPHINCS_BYTES];
	unsigned char root[SPHINCS_BYTES];
	unsigned char * addr = R + SPHINCS_BYTES; /* Convenient pointer */

	/* Retrieves secret pseudorandom value R with sk_2 */
	prf(R, message, length, sk + SK_BYTES/2);
	for (i = 0; i < SPHINCS_BYTES + SPHINCS_ADDRESS_BYTES; ++i)
	{
		printf("%02x", R[(i < SPHINCS_ADDRESS_BYTES ? SPHINCS_BYTES + i : i - SPHINCS_ADDRESS_BYTES)]);
	}
	printf("\n");

	/* Computes digest */
	hash_any_n_2n(digest, R, message, length);

	/* Recovers seed at address = (d || R[SPHINCS_BYTES:])*/
	addr[0] = (MSS_LEVELS << 4) | (addr[0] & 0x0f);
	prf_64(seed, addr, sk);

	/* Signs digest with HORST */
	horst_sign_opti(digest, seed, masks);

	/* Computes HORST public key */
	horst_keygen(root, seed, masks);

	/* For all altitudes, signs previous root in a CMSS fashion */
	for (a = 0; a < MSS_LEVELS; ++a)
	{
		/* The last 5 bits from the subtree index becomes the new leaf index */
		addr[0] = (addr[0] & 0x0f);
		if (a != 0) {
			for (i = SPHINCS_ADDRESS_BYTES - 1; i > 0; --i)
			{
				addr[i] = ((addr[i] & 0xe0) >> 5) | ((addr[i - 1] & 0x1f) << 3);
			}
			addr[0] = (a << 4) & 0xf0;
		}
		else {
			addr[0] = (a << 4) | addr[0];
		}

		/* Recovers the leaf index which will sign the previous root */
		leafidx = addr[SPHINCS_ADDRESS_BYTES - 1] & 0x1f;

		/* Recovers all the leaves from subtree at altitude a */
		for (l = 0; l < MSS_TOTAL_LEAVES; ++l)
		{
			/* Recovers seed at address R_2 */
			addr[SPHINCS_ADDRESS_BYTES - 1] = (addr[SPHINCS_ADDRESS_BYTES - 1] & 0xe0) | l;
			prf_64(seed, addr, sk);

			/* Outputs WOTS+ signature of root */
			if (l == leafidx)
			{
				wotsp_sign(root, seed, masks);
			}

			/* Generates WOTS+ public key and compresses it to the root of an L-tree */
			wotsp_keygen(wots_pk, seed, masks);
			TREE_CONSTRUCTION_MASK(i, j, h, WOTS_L, wots_pk, masks);

			/* Stores WOTS+ compressed key as leaf of top tree */
			for (i = 0; i < SPHINCS_BYTES; ++i)
			{
				leaves[l*SPHINCS_BYTES + i] = wots_pk[0*SPHINCS_BYTES + i];
			}
		}

		/* Outputs MSS signature */
		for (i = 0; i < MSS_TREE_HEIGHT; ++i)
		{
			/* Computes authentication id in leafidx */
			if ((leafidx & 1) == 0)
			{
				leafidx += 1;
			}
			else {
				leafidx -= 1;
			}

			/* Prints authentication path */
			if (i < MSS_TREE_HEIGHT - 1)
			{
				for (j = 0; j < SPHINCS_BYTES; ++j)
				{
					printf("%02x", leaves[leafidx*SPHINCS_BYTES + j]);
				}
			}

			/* Constructs tree */
			for (j = 0; j < (1 << (MSS_TREE_HEIGHT - i)); j += 2)
			{
				hash_nn_n_mask(leaves + (j/2)*SPHINCS_BYTES, leaves + (j)*SPHINCS_BYTES,
				               leaves + (j+1)*SPHINCS_BYTES, masks + 2*(WOTS_LOG_L + i)*SPHINCS_BYTES);
			}

			/* Authentication node from next layer */
			leafidx /= 2;
		}

		/* Copies on root so the next WOTS+ signs it */
		for (i = 0; i < SPHINCS_BYTES; ++i)
		{
			seed[i] = 0;
			root[i] = leaves[i + 0*SPHINCS_BYTES];
		}
	}

	return 0;
}
