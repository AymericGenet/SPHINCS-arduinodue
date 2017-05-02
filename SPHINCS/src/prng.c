/*
 * prng.c
 *
 */

#include "parameters.h"
#include "crypto_chacha12.h"

/* Nonce for ChaCha12 */
static unsigned char nonce[CHACHA12_NONCE_BYTES] = {0};

void prng(unsigned char * out, unsigned long long const outlen, unsigned char const seed[SEED_BYTES])
{
	crypto_chacha12(out, outlen, nonce, seed);
}
