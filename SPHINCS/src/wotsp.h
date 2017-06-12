/*
 * wotsp.h
 *
 */

#ifndef WOTS_H_
#define WOTS_H_

#include "parameters.h"

/*
 * Generates WOTS+ public key associated with the provided seed and masks.
 */

int wotsp_keygen(unsigned char y[WOTS_L*SPHINCS_BYTES],
                 unsigned char const seed[SEED_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES]);

/*
 * Prints textbook WOTS+ signature of a digest on stdout with respect to
 * provided seed and masks.
 */

int wotsp_sign(unsigned char const digest[SPHINCS_BYTES],
               unsigned char const seed[SEED_BYTES],
               unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES]);

/*
 * Verifies the validity of a WOTS+ signature for a digest under the provided
 * public key and masks.
 */

int wotsp_verify(unsigned char const digest[SPHINCS_BYTES],
                 unsigned char const y[WOTS_L*SPHINCS_BYTES],
                 unsigned char const sig[WOTS_L*SPHINCS_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES]);

#endif /* WOTS_H_ */
