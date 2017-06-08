/*
 * prng.c
 *
 */

#include "parameters.h"
#include "crypto_chacha12.h"
#include "ecrypt-sync.h"
#include <string.h>

/* Nonce for ChaCha12 */
static unsigned char nonce[CHACHA12_NONCE_BYTES] = {0};

void prng(unsigned char * out, unsigned long long const outlen, unsigned char const seed[SEED_BYTES])
{
	ECRYPT_ctx tmp;
	ECRYPT_keysetup(&tmp, seed, 8*CHACHA12_KEY_BYTES, 8*CHACHA12_NONCE_BYTES);
	ECRYPT_ivsetup(&tmp, nonce);
	ECRYPT_keystream_bytes(&tmp, out, outlen);
}

void prng_context_setup(ECRYPT_ctx * ctx, unsigned char const seed[SEED_BYTES])
{
	ECRYPT_keysetup(ctx, seed, 8*CHACHA12_KEY_BYTES, 8*CHACHA12_NONCE_BYTES);
	ECRYPT_ivsetup(ctx, nonce);
}

void prng_next(ECRYPT_ctx * ctx, unsigned char out[PRNG_BYTES])
{
	ECRYPT_keystream_bytes(ctx, out, PRNG_BYTES);
}

void prng_context_delete(ECRYPT_ctx * ctx)
{
	memset(ctx->input, 0, sizeof(ctx->input));
}
