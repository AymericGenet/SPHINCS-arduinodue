/*
 * hash.h
 *
 */

#ifndef HASH_H_
#define HASH_H_

#include "parameters.h"

/*
 * Hashes messages of any bytes to digest of SPHINCS_BYTES bytes.
 */

int hash_any_n(unsigned char * out, unsigned char const * in,
               unsigned long const inlen);

/*
 * Hashes messages of any bytes to digest of 2*SPHINCS_BYTES bytes.
 */

int hash_any_2n(unsigned char * out, unsigned char const * in,
                unsigned long const inlen);

/*
 * Hashes messages of SPHINCS_BYTES bytes to digest of SPHINCS_BYTES bytes.
 *
 * The method specified by the SPHINCS documentation starts by expanding the
 * initial input with the bytes of the literal ASCII representation of the
 * string "expand 32-byte to 64-byte state!". Then, it makes use of the
 * permutation step from the stream cipher ChaCha12 and truncates the
 * SPHINCS_BYTES first bytes of the result.
 */

int hash_n_n(unsigned char * out, unsigned char const * in);

/*
 * Hashes messages of SPHINCS_BYTES bytes to digest of SPHINCS_BYTES bytes, as
 * it is done with hash_n_n(), but initially XOR the input with the provided
 * mask.
 */

int hash_n_n_mask(unsigned char * out, unsigned char const * in,
                  unsigned char const * mask);

/*
 * Hashes messages of 2*SPHINCS_BYTES bytes to digest of SPHINCS_BYTES bytes.
 *
 * The method specified by the SPHINCS documentation starts by splitting the
 * input in two: (M1 || M2).
 * 
 * The first input M1 is expanded with the bytes of the literal ASCII
 * representation of the string "expand 32-byte to 64-byte state!", while the
 * second input M2 is expanded with 32 bytes equal to 0. Then, the method
 * applies the permutation step from the stream cipher ChaCha12 on the expanded
 * M1, XOR the output with the the expanded M2, and re-applies the same
 * permutation step on the result. The method finishes by truncating the
 * first SPHINCS_BYTES of the outcome from this whole process.
 */

int hash_2n_n(unsigned char * out, unsigned char const * in);

/*
 * Hashes messages of 2*SPHINCS_BYTES bytes to digest of SPHINCS_BYTES bytes,
 * as it is done with hash_2n_n(), but initially XOR the input with the
 * provided mask.
 */

int hash_2n_n_mask(unsigned char * out, unsigned char const * in,
                   unsigned char const * mask);

/*
 * Hashes the concatenation of two messages of SPHINCS_BYTES bytes, resulting
 * in a message of 2*SPHINCS_BYTES bytes.
 *
 * The method basically concatenates the two inputs in1 and in2 together and
 * uses hash_2n_n().
 */

int hash_nn_n(unsigned char * out, unsigned char const * in1,
              unsigned char const * in2);

/*
 * Hashes the concatenation of two messages of SPHINCS_BYTES bytes, resulting
 * in a message of 2*SPHINCS_BYTES bytes, as it is done with hash_nn_n(), but
 * initially XOR the inputs with the provided mask.
 */

int hash_nn_n_mask(unsigned char * out, unsigned char const * in1,
                   unsigned char const * in2, unsigned char const * mask);

/*
 * Hashes a message of SPHINCS_BYTES bytes in chain (chainlen times) to a
 * digest of SPHINCS_BYTES bytes.
 */

int hash_chain_n(unsigned char * out, unsigned char const * in,
                 unsigned long const chainlen);

/*
 * Hashes a message of SPHINCS_BYTES bytes in chain (chainlen times) to a
 * digest of SPHINCS_BYTES bytes, but XOR each step with the provided mask.
 */

int hash_chain_n_mask(unsigned char * out, unsigned char const * in,
                      unsigned char const * masks, unsigned long const chainlen);

#endif /* HASH_H_ */
