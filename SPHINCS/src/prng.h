/*
 * prng.h
 *
 */

#include <sys/types.h>
#include "ecrypt-sync.h"
#include "parameters.h"

#ifndef PRNG_H_
#define PRNG_H_

void prng(unsigned char * out, unsigned long long const outlen, unsigned char const seed[SEED_BYTES]);

void prng_context_setup(ECRYPT_ctx * ctx, unsigned char const seed[SEED_BYTES]);

void prng_next(ECRYPT_ctx * ctx, unsigned char out[2*SPHINCS_BYTES]);

void prng_context_delete(ECRYPT_ctx * ctx);

#endif /* PRNG_H_ */