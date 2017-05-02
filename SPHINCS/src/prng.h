/*
 * prng.h
 *
 */

#include <sys/types.h>

#ifndef PRNG_H_
#define PRNG_H_

void prng(unsigned char * out, unsigned long long const outlen, unsigned char const seed[SEED_BYTES]);

#endif /* PRNG_H_ */