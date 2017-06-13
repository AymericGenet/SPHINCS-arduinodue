/*
 * prf.h
 *
 */

#include "parameters.h"

#ifndef PRF_H_
#define PRF_H_

/*
 * Applies ensemble of pseudorandom functions to the concatenation of in1 and
 * in2.
 *
 * The purpose of this function is to derive pseudorandom seeds for the
 * generation of WOTS+ secret values, where in1 is the address of the WOTS+
 * leaf in 64 bits, and in2 the first SPHINCS secret seed sk1.
 */

int prf_64(unsigned char out[SPHINCS_BYTES],
           unsigned char const in1[SPHINCS_ADDRESS_BYTES],
           unsigned char const in2[SEED_BYTES]);

/*
 * Applies family of pseudorandom functions to the concatenation of in2 and
 * in1.
 *
 * This function actually outputs two secret values (R_1, R_2). The first one
 * is used to compute the actual digest of the message, and the second one is
 * used to select the HORST instance, where in1 is the message and in2 the
 * second secret seed sk2.
 */

int prf(unsigned char out[2*SPHINCS_BYTES],
        unsigned char const * in1,
        unsigned long long in1len,
        unsigned char const in2[SEED_BYTES]);

#endif /* PRF_H_ */
