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
#include "misc.h"

static ECRYPT_ctx ctx;
static unsigned char tmp[PRNG_BYTES];

static int horst_leafcalc(struct Node * node, unsigned int leaf)
{
	if (leaf >= HORST_T)
	{
		return 0; /* False */
	}
	else {
		if ((leaf & 1) == 0) /* cheap mod 2*/
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


int horst_keygen(unsigned char y[SPHINCS_BYTES],
                 unsigned char const seed[SEED_BYTES],
                 unsigned char const masks[HORST_TAU*SPHINCS_BYTES])
{
	int i = 0;
	struct Node node;
	struct Stack stack;
	stack.index = -1;

	prng_context_setup(&ctx, seed);

	/* Constructs the Merkle tree root with treehash */
	horst_leafcalc(&node, 0);
	l_treehash_mask(&node, &stack, 2*(HORST_T - 1), 1, horst_leafcalc, masks);

	prng_context_delete(&ctx);

	/* Copies on the public-key */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		y[i] = node.hash[i];
	}

	return 0;
}

int horst_sign(unsigned char const digest[SPHINCS_DIGEST_BYTES],
               unsigned char const seed[SEED_BYTES],
               unsigned char const masks[HORST_TAU*SPHINCS_BYTES])
{
	unsigned int i = 0, j = 0, k = 0, leaf = 0;
	ECRYPT_ctx ctx_secret;
	struct Node node;
	struct Stack stack;
	stack.index = -1;
	unsigned int auth_path_id[HORST_TAU];
	unsigned char auth_path_hash[HORST_TAU*SPHINCS_BYTES];
	unsigned int blocks[HORST_K];

	/* Splits the message digest in HORST_K blocks of HORST_TAU bits */
	for (i = 0; i < HORST_K; ++i)
	{
		SPLIT_16(i, blocks[i], digest);
	}

	/* Sorts the blocks in ascending order */
	qsort(blocks, HORST_K, sizeof(unsigned int), uint_cmp);

	/* Outputs the secret values that correspond to the blocks in one PRNG traversal */
	prng_context_setup(&ctx_secret, seed);
	for (i = 0; i < HORST_K; ++i)
	{
		/* Prepares the treehash */
		compute_auth_path_id(auth_path_id, HORST_TAU, blocks[i]);
		leaf = 0;
		prng_context_setup(&ctx, seed);
		horst_leafcalc(&node, leaf++);

		/* Computes the authentication path associated with block id */
		for (j = 0; j < 2*(HORST_T - 1); ++j)
		{
			leaf = l_treehash_mask(&node, &stack, 1, leaf, horst_leafcalc, masks);
			if (auth_path_id[node.level] == node.id)
			{
				for (k = 0; k < SPHINCS_BYTES; ++k)
				{
					auth_path_hash[k + node.level*SPHINCS_BYTES] = node.hash[k];
				}
			}
		}

		/* Offset in-place:
		 *   if i == 0: draw data until you get to the correct secret value
		 *   else: draw the difference of data to get to the next secret value */
		for (j = 0; j < (i == 0 ? blocks[i]/2 + 1 : blocks[i]/2 - blocks[i-1]/2); ++j)
		{
			prng_next(&ctx_secret, tmp);
		}

		/* Outputs secret value on the stdout */
		for (j = 0; j < SPHINCS_BYTES; ++j)
		{
			printf("%02x", tmp[j + (blocks[i] & 1)*SPHINCS_BYTES]);
			/* Erases it asap */
			tmp[j] = 0;
			tmp[j + SPHINCS_BYTES] = 0;
		}

		/* Outputs authentication path on the stdout */
		for (j = 0; j < HORST_TAU; ++j)
		{
			for (k = 0; k < SPHINCS_BYTES; ++k)
			{
				printf("%02x", auth_path_hash[k + j*SPHINCS_BYTES]);
			}
		}

		prng_context_delete(&ctx);
	}

	return 0;
}

int horst_sign_opti(unsigned char const digest[SPHINCS_DIGEST_BYTES],
                    unsigned char const seed[SEED_BYTES],
                    unsigned char const masks[HORST_TAU*SPHINCS_BYTES])
{
	unsigned int i = 0, j = 0, leaf = 0;
	struct Node node;
	struct Stack stack;
	stack.index = -1;
	unsigned int auth_path_id[HORST_MAX_LEVEL*HORST_K];
	unsigned int blocks[HORST_K];

	/* Splits the message digest in HORST_K blocks of HORST_TAU bits */
	for (i = 0; i < HORST_K; ++i)
	{
		SPLIT_16(i, blocks[i], digest);
	}

	/* Sorts the blocks in ascending order */
	qsort(blocks, HORST_K, sizeof(unsigned int), uint_cmp);

	/* Outputs the secret values that correspond to the blocks in one PRNG traversal */
	prng_context_setup(&ctx, seed);
	for (i = 0; i < HORST_K; ++i)
	{
		/* Offset in-place:
		*   if i == 0: draw data until you get to the correct secret value
		*   else: draw the difference of data to get to the next secret value */
		for (j = 0; j < (i == 0 ? blocks[i]/2 + 1 : blocks[i]/2 - blocks[i-1]/2); ++j)
		{
			prng_next(&ctx, tmp);
		}

		/* Outputs secret value on the stdout */
		printf("(sk=%i) ", blocks[i]);
		for (j = 0; j < SPHINCS_BYTES; ++j)
		{
			printf("%02x", tmp[j + (blocks[i] & 1)*SPHINCS_BYTES]);
			/* Erases it asap */
			tmp[j] = 0;
			tmp[j + SPHINCS_BYTES] = 0;
		}
		printf("\n");

		/* Computes authentication path associated with block id */
		compute_auth_path_id(auth_path_id + i*HORST_MAX_LEVEL, HORST_MAX_LEVEL, blocks[i]);
	}

	/* Prepares the treehash */
	prng_context_setup(&ctx, seed);
	horst_leafcalc(&node, leaf++);

	/* Computes the authentication path in one treehash */
	for (i = 0; i < HORST_TRUNC_TREEHASH_ROUNDS; ++i)
	{
		leaf = l_treehash_mask(&node, &stack, 1, leaf, horst_leafcalc, masks);

		/* Outputs node with position if node needs to be printed */
		if (node.level == HORST_MAX_LEVEL || is_in_auth_path(node, auth_path_id))
		{
			printf("(h=%d, id=%d) ", node.level, node.id);
			for (j = 0; j < SPHINCS_BYTES; ++j)
			{
				printf("%02x", node.hash[j]);
			}
			printf("\n");

			/* FIXME: skip node in treehash (hack) */
			if (node.level == HORST_MAX_LEVEL && !horst_leafcalc(&node, leaf++))
			{
				break;
			}
		}
	}

	prng_context_delete(&ctx);

	return 0;
}

int horst_verify_opti(unsigned char const digest[SPHINCS_DIGEST_BYTES],
                      unsigned char const y[SPHINCS_BYTES],
                      int (*sig_stream)(void),
                      unsigned char const masks[HORST_TAU*SPHINCS_BYTES])
{
	unsigned char sk_hash[SPHINCS_BYTES*HORST_K];
	unsigned char top_layer[SPHINCS_BYTES*HORST_MAX_LEVEL_NODES];
	unsigned int blocks[HORST_K];
	unsigned int top_layer_idx = 0; /* NOTE: This counter is shifted by 1. */
	int i = 0, j = 0, sk_idx = 0;
	struct Node node;
	struct Stack stack;
	stack.index = -1;

	/* Splits the message digest in HORST_K blocks of HORST_TAU bits */
	for (i = 0; i < HORST_K; ++i)
	{
		SPLIT_16(i, blocks[i], digest);
	}

	/* Sorts the blocks in ascending order */
	qsort(blocks, HORST_K, sizeof(unsigned int), uint_cmp);

	/* Parse first value */
	for (i = 0; i < HORST_K; ++i)
	{
		if (!parse_sk(sk_hash + i*SPHINCS_BYTES, sig_stream))
		{
			return 0; /* FIXME: goto fail */
		}
		hash_n_n(sk_hash + i*SPHINCS_BYTES, sk_hash + i*SPHINCS_BYTES);
	}

	/*
	 * Re-arranged treehash which reads authentication path from sig_stream()
	 */
	while (top_layer_idx < HORST_MAX_LEVEL_NODES + 1 && parse_node(&node, sig_stream))
	{
		/* If node is a leaf, merges it with the next sk_hash */
		if (node.level == 0)
		{
			if (sk_idx >= HORST_K)
			{
				return 0; /* FIXME: goto fail */
			}

			if ((node.id & 1) == 0)
			{
				if (blocks[sk_idx] != node.id + 1)
				{
					return 0; /* False */
				}
				hash_nn_n_mask(node.hash, node.hash, sk_hash + sk_idx*SPHINCS_BYTES, masks);
			}
			else
			{
				if (blocks[sk_idx] != node.id - 1)
				{
					return 0; /* False */
				}
				hash_nn_n_mask(node.hash, sk_hash + sk_idx*SPHINCS_BYTES, node.hash, masks);
			}

			node.level = 1;
			node.id /= 2;
			sk_idx++;
		}
		/* If node is top node and was already computed, compare them */
		else if (node.level == HORST_MAX_LEVEL && node.id < top_layer_idx)
		{
			for (i = 0; i < SPHINCS_BYTES; ++i)
			{
				if (node.hash[i] != top_layer[i + SPHINCS_BYTES*node.id])
				{
					return 0; /* False */
				}
			}
		}

		/* While two nodes can be merged, merge them */
		while (stack.index >= 0 && node.level != HORST_MAX_LEVEL
		       && stack.nodes[stack.index].level == node.level
		       && stack.nodes[stack.index].id != node.id)
		{
			if ((node.id & 1) == 0)
			{
				hash_nn_n_mask(node.hash, node.hash, stack.nodes[stack.index].hash,
				               masks + 2*(node.level)*SPHINCS_BYTES);
			}
			else
			{
				hash_nn_n_mask(node.hash, stack.nodes[stack.index].hash, node.hash,
				               masks + 2*(node.level)*SPHINCS_BYTES);
			}
			node.level++;
			node.id = (stack.nodes[stack.index].id)/2;
			stack.index--; /* Short stack pop */
		}

		/* If node top level, store it for root computation */
		if (node.level == HORST_MAX_LEVEL)
		{
			/* If not already stored */
			if ((node.id + 1) > top_layer_idx)
			{
				for (i = 0; i < SPHINCS_BYTES; ++i)
				{
					top_layer[i + node.id*SPHINCS_BYTES] = node.hash[i];
				}
				top_layer_idx = node.id + 1;
			}
		}
		/* If node is not top node and not a duplicate, push it on the stack */
		else if (stack.nodes[stack.index].level != node.level || stack.nodes[stack.index].id != node.id)
		{
			stack.index++;
			stack.nodes[stack.index] = node;
		}
	}

	/* Computes tree root from top layer */
	TREE_CONSTRUCTION_MASK(i, j, top_layer_idx, HORST_MAX_LEVEL_NODES, top_layer,
	                       masks + 2*HORST_MAX_LEVEL*SPHINCS_BYTES);

	/* Compare tree root with public key */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		if (top_layer[i] != y[i])
		{
			return 0; /* False */
		}
	}

	return 1; /* True */
}
