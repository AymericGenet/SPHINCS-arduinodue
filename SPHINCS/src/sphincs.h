/*
 * sphincs.h
 *
 */

#include "parameters.h"

#ifndef SPHINCS_H_
#define SPHINCS_H_

/*
 * Generates SPHINCS public key associated with the provided seed sk1 and
 * masks.
 */

int sphincs_keygen(unsigned char pk[PK_BYTES],
                   unsigned char const sk1[SEED_BYTES],
                   unsigned char const masks[MASKS_BYTES]);

/*
 * Prints SPHINCS signature of a digest on stdout with respect to provided seed
 * and masks.
 */

int sphincs_sign(unsigned char const * message,
                 unsigned long long length,
                 unsigned char const sk[SK_BYTES],
                 unsigned char const masks[MASKS_BYTES]);

/*
 * Verifies the validity of a SPHINCS signature for a digest under the provided
 * public key and masks.
 */

int sphincs_verify(unsigned char const * message,
                   unsigned long long length,
                   unsigned char const pk[SPHINCS_BYTES],
                   int (*sig_stream)(void),
                   unsigned char const masks[MASKS_AMOUNT*SPHINCS_BYTES]);

#endif /* SPHINCS_H_ */
