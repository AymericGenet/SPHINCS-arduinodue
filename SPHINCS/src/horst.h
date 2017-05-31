/*
 * horst.h
 *
 */

#include "parameters.h"

#ifndef HORST_H_
#define HORST_H_

/*
 * Prints public key Y associated with secret key X which is generated using
 * seed sk1 with masks for a WOTS+ instance.
 */

int horst_keygen(unsigned char const seed[SEED_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES]);

/*
 * Signs a message according to WOTS+ scheme with respect to secret key sk1 and
 * masks.
 */

int horst_sign(unsigned char const digest[SPHINCS_BYTES],
               unsigned char const seed[SEED_BYTES],
               unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES]);

/*
 * Verifies the validity of a signature for a message according to WOTS+ scheme
 * under the public key pk and masks.
 */

int horst_verify(unsigned char const digest[SPHINCS_BYTES],
                 unsigned char const pk[WOTS_L*SPHINCS_BYTES],
                 unsigned char const sig[WOTS_L*SPHINCS_BYTES],
                 unsigned char const masks[WOTS_MAX_INT*SPHINCS_BYTES]);

#endif /* HORST_H_ */
