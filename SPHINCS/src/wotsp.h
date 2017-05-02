/*
 * wotsp.h
 *
 */

#ifndef WOTS_H_
#define WOTS_H_

#include "parameters.h"

/*
 * Generates a key pair (X, Y) with masks for a WOTS+ instance.
 */

int wotsp_keygen(unsigned char y[WOTS_L*SPHINCS_BYTES],
                 unsigned char x[WOTS_L*SPHINCS_BYTES],
                 unsigned char const sk1[SEED_BYTES],
                 unsigned char const masks[(WOTS_MAX_INT-1)*SPHINCS_BYTES]);

/*
 * Signs a message according to WOTS+ scheme with respect to secret key sk and
 * masks.
 */

int wotsp_sign(unsigned char const message[SPHINCS_BYTES],
               unsigned char const sk[SEED_BYTES],
               unsigned char const masks[(WOTS_MAX_INT-1)*SPHINCS_BYTES]);

/*
 * Verifies the validity of a signature for a message according to WOTS+ scheme
 * under the public key pk and masks.
 */

int wotsp_verify(unsigned char pk[WOTS_L*SPHINCS_BYTES],
                 unsigned char const sig[WOTS_L*SPHINCS_BYTES],
                 unsigned char const message[SPHINCS_BYTES],
                 unsigned char const masks[(WOTS_MAX_INT-1)*SPHINCS_BYTES]);

#endif /* WOTS_H_ */
