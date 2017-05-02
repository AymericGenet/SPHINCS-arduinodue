/*
 * crypto_chacha12.c
 *
 */

#include "crypto_chacha12.h"
#include "ecrypt-sync.h"

int crypto_chacha12(unsigned char * out, unsigned long long const outlen,
                    unsigned char const * nonce, unsigned char const * key)
{
	ECRYPT_ctx tmp;
	ECRYPT_keysetup(&tmp, key, 8*CHACHA12_KEY_BYTES, 8*CHACHA12_NONCE_BYTES);
	ECRYPT_ivsetup(&tmp, nonce);
	ECRYPT_keystream_bytes(&tmp, out, outlen);

	return 0;
}
