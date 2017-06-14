/*
 * utils.h
 *
 */

#include "parameters.h"

#ifndef UTILS_H_
#define UTILS_H_

/* in1 : "" (empty string) */
#define IN1_LEN 0
extern unsigned char const in1[IN1_LEN + 1];
extern unsigned char const in1_hashed_256[SPHINCS_BYTES];
extern unsigned char const in1_hashed_512[2*SPHINCS_BYTES];

/* in2 : "The quick brown fox jumps over the lazy dog" (43 characters) */
#define IN2_LEN 43
extern unsigned char const in2[IN2_LEN + 1];
extern unsigned char const in2_hashed_256[SPHINCS_BYTES];
extern unsigned char const in2_hashed_512[2*SPHINCS_BYTES];

/* in_sk : 2x32 bytes of random data */
extern unsigned char const in_sk[SK_BYTES];

/* masks : 1024 bytes of random data */
extern unsigned char const in_masks[MASKS_BYTES];

/* stream : 128 bytes from ChaCha12 stream */
#define PRNG_STREAM_LEN 128
extern unsigned char const prng_stream[PRNG_STREAM_LEN];

/* address : 8 bytes of valid HORST addressing */
extern unsigned char const prf_address[SPHINCS_ADDRESS_BYTES];

/*
 * Returns 1 (True) if the content from arr1 is the same as in arr2 up to len,
 * or 0 (False) otherwise.
 */

int array_cmp(unsigned char const * arr1, unsigned char const * arr2, int len);

/*
 * Returns 1 (True) if the content from arr is zero up to len, or 0 (False)
 * otherwise.
 */

int array_is_zero(unsigned int const * arr, int len);

#endif /* UTILS_H_ */
