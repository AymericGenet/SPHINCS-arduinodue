/*
 * wotsp.h
 *
 */

#ifndef WOTS_H_
#define WOTS_H_

#include "parameters.h"

/*
 * Prints public key Y associated with secret key X which is generated using
 * seed sk1 with masks for a WOTS+ instance.
 */

int wotsp_keygen(unsigned char const sk1[SEED_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES]);

/*
 * Signs a message according to WOTS+ scheme with respect to secret key sk1 and
 * masks.
 */

int wotsp_sign(unsigned char const digest[SPHINCS_BYTES],
               unsigned char const sk1[SEED_BYTES],
               unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES]);

/*
 * Verifies the validity of a signature for a message according to WOTS+ scheme
 * under the public key pk and masks.
 */

int wotsp_verify(unsigned char const digest[SPHINCS_BYTES],
                 unsigned char const y[WOTS_L*SPHINCS_BYTES],
                 unsigned char const sig[WOTS_L*SPHINCS_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES]);

#endif /* WOTS_H_ */
