/*
 * trees.c
 *
 */

#include <stdlib.h>
#include <string.h>
#include "trees.h"
#include "hash.h"

/*
 * Mocks the push operation on top of the stack.
 */

static int stack_push(struct Stack * stack, struct Node * node)
{
	if (stack->index + 1 >= STACK_MAX_SIZE)
	{
		return 0; /* false */
	}
	stack->index++;
	stack->nodes[stack->index] = (*node);
	return 1; /* true */
}

/*
 * Mocks the pop operation from the top of the stack.
 */

static int stack_pop(struct Stack * stack, struct Node * node)
{
	if (stack->index < 0)
	{
		return 0; /* false */
	}
	(*node) = stack->nodes[stack->index];
	stack->index--;
	return 1; /* true */
}

/*
 * Hashes the concatenation of the node from the top of stack (by popping it)
 * with the node provided in parameter, and put the result in node.
 */

static void hash_stacknode_node(struct Node * node, struct Stack * stack)
{
	hash_nn_n(node->hash, stack->nodes[stack->index].hash, node->hash);
	node->level++;
	node->id = (stack->nodes[stack->index].id)/2;
	stack->index--; /* Short stack pop. */
}

/*
 * Hashes the concatenation of the node from the top of stack (by popping it)
 * with the node provided in parameter with masks, and put the result in node.
 */

static void hash_stacknode_node_mask(struct Node * node, struct Stack * stack,
                                     unsigned char const * mask)
{
	hash_nn_n_mask(node->hash, stack->nodes[stack->index].hash, node->hash, mask);
	node->level++;
	node->id = (stack->nodes[stack->index].id)/2;
	stack->index--; /* Short stack pop. */
}

unsigned int l_treehash(struct Node * node, struct Stack * stack,
                        unsigned long depth, unsigned int leaf,
                        int (*leafcalc)(struct Node * node, unsigned int))
{
	while (depth != 0)
	{
		/* If the node on the stack is at the same level as the current node. */
		if (stack->index >= 0 && (node->level) == stack->nodes[stack->index].level)
		{
			/* Merge them together. */
			hash_stacknode_node(node, stack);
		}
		/* Otherwise, push on the stack and pick a new leaf */
		else
		{
			if (!stack_push(stack, node))
			{
				return -1; /* FIXME: goto fail */
			}
			/*
			 * Try to pick up a new leaf.
			 * If failed, merge the remaining nodes together.
			 */
			if (!leafcalc(node, leaf++))
			{
				if (!stack_pop(stack, node))
				{
					return -1; /* FIXME: goto fail */
				}
				if (stack->index >= 0)
				{
					/* Handle the case of an L-tree */
					node->level = stack->nodes[stack->index].level;
					hash_stacknode_node(node, stack);
				}
			}
		}

		depth--;
	}
	return leaf;
}

unsigned int l_treehash_mask(struct Node * node, struct Stack * stack,
                             unsigned long depth, unsigned int leaf,
                             int (*leafcalc)(struct Node * node, unsigned int),
                             unsigned char const * masks)
{
	while (depth != 0)
	{
		/* If the node on the stack is at the same level as the current node. */
		if (stack->index >= 0 && (node->level) == stack->nodes[stack->index].level)
		{
			/* Merge them together. */
			hash_stacknode_node_mask(node, stack, masks + 2*(node->level)*SPHINCS_BYTES);
		}
		/* Otherwise, push on the stack and pick a new leaf */
		else
		{
			if (!stack_push(stack, node))
			{
				return -1; /* FIXME: goto fail */
			}
			/*
			 * Try to pick up a new leaf.
			 * If failed, merge the remaining nodes together.
			 */
			if (!leafcalc(node, leaf++))
			{
				if (!stack_pop(stack, node))
				{
					return -1; /* FIXME: goto fail */
				}
				if (stack->index >= 0)
				{
					/* Handle the case of an L-tree */
					node->level = stack->nodes[stack->index].level;
					hash_stacknode_node_mask(node, stack, masks + 2*(node->level)*SPHINCS_BYTES);
				}
			}
		}

		depth--;
	}
	return leaf;
}

void compute_auth_path_id(unsigned int * auth_path_id,
                          unsigned int const length, unsigned int block)
{
	int i = 0;

	for (i = 0; i < length; ++i)
	{
		if ((block & 1) == 0)
		{
			auth_path_id[i] = block + 1;
		}
		else
		{
			auth_path_id[i] = block - 1;
		}
		block /= 2;
	}
}

int is_in_auth_path(struct Node const node, unsigned int const * auth_path_id)
{
	int i = 0;

	for (i = 0; i < HORST_K; ++i)
	{
		if (auth_path_id[node.level + i*HORST_MAX_LEVEL] == node.id)
		{
			return 1; /* True */
		}
	}

	return 0; /* False */
}
