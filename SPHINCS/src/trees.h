/*
 * trees.h
 *
 */

#include "parameters.h"

#ifndef TREES_H_
#define TREES_H_

/* The stack cannot contain more than one node at each level */
#define STACK_MAX_SIZE MSS_TREE_HEIGHT

/*
 * Node structure which contains the hash of a node in a tree and its level
 * of altitude in the tree.
 */

struct Node {
	unsigned char hash[SPHINCS_BYTES];
	unsigned char level;
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
 * (i.e. in increasing order) with the hash_nn_n() function in order to
 * ultimately compute its root. If the tree is not balanced, as it is the case
 * with an L-tree, the level of the last leaves are automatically adjusted such
 * that the root is always correctly computed.
 * 
 * In order to merge the nodes from the whole tree together, the function needs
 * a stack to store the computations it has previously made, which is passed as
 * another parameter. The stack is automatically updated and do not need any
 * operation from the outside.
 
 * The function will make an amount of steps equals to depth and put the result
 * of the current step in node. When the function cannot merge two nodes
 * together, it will pick up a leaf corresponding to the id passed in argument
 * by calling the leafcalc() function provided with it.
 *
 * In order to use this function, the node MUST be initialized to the first
 * call of the leafcalc() function, and the stack must be empty with its index
 * set to -1. The behavior when NULL pointers are given is undefined.
 *
 * The returning value corresponds to the next leaf to be picked in the
 * leafcalc() function. If a new leaf has been picked up, the value returned
 * needs to be used in the next call of l_treehash().
 *
 * We note that Merkle trees are a specific case of L-trees ; the function can
 * thus be used for them as well.
 */

unsigned int l_treehash(struct Node * node, struct Stack * stack,
                        unsigned long depth, unsigned int leaf,
                        int (*leafcalc)(struct Node * node, unsigned int));

/*
 * Constructs an L-tree in a treehash fashion, as in l_treehash(), but merges
 * two nodes on a tree next to each other on the same level from left to right
 * (i.e. in increasing order) with the hash_nn_n_mask() function in order to
 * ultimately compute its root.
 *
 * See l_treehash() documentation.
 */

unsigned int l_treehash_mask(struct Node * node, struct Stack * stack,
                             unsigned long depth, unsigned int leaf,
                             int (*leafcalc)(struct Node * node, unsigned int),
                             unsigned char const * masks);


#endif /* TREES_H_ */
