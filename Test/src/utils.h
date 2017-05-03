/*
 * utils.h
 *
 */

#include "parameters.h"

#ifndef UTILS_H_
#define UTILS_H_

/************************************************************************/
/*                           Inputs reference                           */
/************************************************************************/

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

/* sk1, sk2 : 2x32 bytes of random data */
extern unsigned char const in_sk1[SEED_BYTES];
extern unsigned char const in_sk2[SEED_BYTES];

/* masks : 1024 bytes of random data */
extern unsigned char const in_masks[MASKS_BYTES];

/*
 * Compares two arrays.
 */

int array_cmp(unsigned char const * arr1, unsigned char const * arr2, int len);

#endif /* UTILS_H_ */