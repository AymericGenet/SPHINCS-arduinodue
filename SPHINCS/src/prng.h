/*
 * prng.h
 *
 */

#include <sys/types.h>
#include "ecrypt-sync.h"
#include "parameters.h"

#ifndef PRNG_H_
#define PRNG_H_

/*
 * Generates outlen pseudorandom number with provided seed.
 */

void prng(unsigned char * out, unsigned long long const outlen, unsigned char const seed[SEED_BYTES]);

/*
 * Sets up a pseudorandom number generator context with provided seed.
 */

void prng_context_setup(ECRYPT_ctx * ctx, unsigned char const seed[SEED_BYTES]);

/*
 * Generates the next PRNG_BYTES bytes of pseudorandom number from context.
 */

void prng_next(ECRYPT_ctx * ctx, unsigned char out[PRNG_BYTES]);

/*
 * Deletes pseudorandom number generator context.
 */

void prng_context_delete(ECRYPT_ctx * ctx);

#endif /* PRNG_H_ */
