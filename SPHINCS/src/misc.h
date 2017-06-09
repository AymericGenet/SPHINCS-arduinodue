/*
 * misc.h
 *
 */

#include "trees.h"

#ifndef MISC_H_
#define MISC_H_

#define SPLIT_16(i, m, digest) do { \
	m = digest[2*i]; /* unsure about long promotion */ \
	m = (m << 8) + digest[2*i + 1]; \
} while (0)

/*
 * Compares two unsigned int.
 */

int uint_cmp(const void * a, const void * b);

/*
 * Parses a secret value from sig_stream.
 *
 * The lines from the stream must conform to the following format:
 *
 *     "(sk=%ui) %s\n"
 *
 * where %s is an hexstring of size 2*SPHINCS_BYTES.
 */

int parse_sk(unsigned char * sk_hash, int (*sig_stream)());

/*
 * Parses a node from sig_stream.
 *
 * The lines from the stream must conform to the following format:
 *
 *     "(h=%uc, id=%ui) %s\n"
 *
 * where %s is an hexstring of size 2*SPHINCS_BYTES.
 */

int parse_node(struct Node * node, int (*sig_stream)());

#endif /* MISC_H_ */
