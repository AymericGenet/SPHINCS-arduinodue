/*
 * trees.h
 *
 */

#include "parameters.h"
#include <string.h>

#ifndef TREES_H_
#define TREES_H_

/* The stack cannot contain more than one node at each level */
#define STACK_MAX_SIZE HORST_TAU

#define INT_CEIL(x, y) 1 + ((x - 1) / y)

#define TREE_CONSTRUCTION(i, j, total, leaves) do { \
	for (i = total; i > 1; i = INT_CEIL(i, 2)) { \
		for (j = 0; j < i; j += 2) { \
			if (j + 2 > i) { \
				memcpy(leaves + (j/2)*SPHINCS_BYTES, leaves + (j)*SPHINCS_BYTES, SPHINCS_BYTES); \
			} else { \
				hash_nn_n(leaves + (j/2)*SPHINCS_BYTES, leaves + (j)*SPHINCS_BYTES, \
				          leaves + (j+1)*SPHINCS_BYTES); \
			} \
		} \
	} \
} while (0)

#define TREE_CONSTRUCTION_MASK(i, j, h, total, leaves, masks) do { \
	for (i = total, h = 0; i > 1; i = INT_CEIL(i, 2), h++) { \
		for (j = 0; j < i; j += 2) { \
			if (j + 2 > i) { \
				memcpy(leaves + (j/2)*SPHINCS_BYTES, leaves + (j)*SPHINCS_BYTES, SPHINCS_BYTES); \
			} else { \
				hash_nn_n_mask(leaves + (j/2)*SPHINCS_BYTES, leaves + (j)*SPHINCS_BYTES, \
				               leaves + (j+1)*SPHINCS_BYTES, \
				               masks + (2*h)*SPHINCS_BYTES); \
			} \
		} \
	} \
} while(0)

/*
 * Node structure which contains the hash of a node in a tree, its level of
 * altitude in the tree, and its unique id on its level.
 *
 * The id is used to recognize nodes during the treehash algorithm, so the
 * authentication path can be deduced.
 *
 * The default values for the hash is either the root of the Merkle L-tree
 * constructed from a WOTS+ verification key, or the root of a HORST Merkle
 * tree. The level starts with 0, up to the height of the Merkle tree (which,
 * then, corresponds to the root), and the starting id begins with the leaf id.
 */

struct Node {
	unsigned char hash[SPHINCS_BYTES];
	unsigned char level;
	unsigned int id;
};

/*
 * Stack structure which stores previously computed nodes for further
 * computation. The struct mocks a stack with an array of nodes and the
 * current index in the stack.
 */

struct Stack {
	struct Node nodes[STACK_MAX_SIZE];
	int index;
};


/*
 * Constructs an L-tree in a treehash fashion. The treehash algorithm merges
 * two nodes on a tree next to each other on the same level from left to right
 * (i.e. in increasing id order) with the hash_nn_n() function in order to
 * ultimately compute its root. If the tree is not balanced, as it may be the
 * case with L-trees, the level of the last leaves are automatically adjusted
 * such that the root is always correctly computed.
 * 
 * In order to merge the nodes from the whole tree together, the function needs
 * a stack to store the computations it has previously made. The minimum size
 * required for the stack is the height of the tree to be computed (times the
 * number of bytes in a hash). The stack is internally updated and does not
 * need any operation from the outside.
 
 * The function will make an amount of steps equals to depth and put the result
 * of the current step in node. When the function cannot merge two nodes
 * together, it will pick up a leaf corresponding to the id passed in argument
 * by calling the leafcalc() function.
 *
 * In order to use this function, the node MUST be initialized to the first
 * call of the leafcalc() function, and the stack must be empty with its index
 * set to -1. The behavior is undefined when NULL pointers are provided.
 *
 * The returning value corresponds to the next leaf to be picked in the
 * leafcalc() function. If a new leaf has been picked up, the value returned
 * needs to be used in the next call of l_treehash().
 *
 * We note that Merkle trees are a specific case of L-trees; the function can
 * thus be used for them as well.
 */

unsigned int l_treehash(struct Node * node, struct Stack * stack,
                        unsigned long depth, unsigned int leaf,
                        int (*leafcalc)(struct Node * node, unsigned int));

/*
 * Constructs an L-tree in a treehash fashion, as in l_treehash(), but merges
 * two nodes on a tree next to each other on the same level from left to right
 * (i.e. in increasing id order) with the hash_nn_n_mask() function in order to
 * ultimately compute its root.
 *
 * See l_treehash() documentation.
 */

unsigned int l_treehash_mask(struct Node * node, struct Stack * stack,
                             unsigned long depth, unsigned int leaf,
                             int (*leafcalc)(struct Node * node, unsigned int),
                             unsigned char const * masks);


/*
 * Computes the id of the authentication path.
 */

void compute_auth_path_id(unsigned int * auth_path_id,
                          unsigned int const length, unsigned int block);


/*
 * Checks if node is in authentication path.
 */

int is_in_auth_path(struct Node const node, unsigned int const * auth_path_id);

#endif /* TREES_H_ */
