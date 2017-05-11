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
	if (stack->index >= STACK_MAX_SIZE)
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
	stack->index--; /* Short stack pop. */
	node->level++;
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
