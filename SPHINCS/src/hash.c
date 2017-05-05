/*
 * hash.c
 *
 */

#include "BLAKE-256.h"
#include "BLAKE-512.h"
#include "crypto_chacha12.h"
#include "parameters.h"
#include "ecrypt-sync.h"

/* Constant chosen by SPHINCS authors */
static char const * hash_const_c = "expand 32-byte to 64-byte state!";

int hash_any_n(unsigned char * out, unsigned char const * in,
               unsigned long const inlen)
{
	crypto_hash_blake256(out, in, inlen);
	return 0;
}

int hash_any_2n(unsigned char * out, unsigned char const * in,
                unsigned long const inlen)
{
	crypto_hash_blake512(out, in, inlen);
	return 0;
}

int hash_n_n(unsigned char * out, unsigned char const * in)
{
	int i;
	u32 tmp[SPHINCS_BYTES/2];

	/* Translate the input in integers and expand with bytes from literal string */
	for (i = 0; i < SPHINCS_BYTES/4; ++i)
	{
		tmp[i] = U8TO32_LITTLE(in + 4*i);
		tmp[i + 8] = U8TO32_LITTLE(hash_const_c + 4*i);
	}

	/* Apply the permutation step of ChaCha12 */
	chacha12_perm_c(tmp, tmp);

	/* Translate the (SPHINCS_BYTES/4) first integers in bytes */
	for (i = 0; i < SPHINCS_BYTES/4; ++i)
	{
		U32TO8_LITTLE(out + 4*i, tmp[i]);
	}

	return 0;
}

int hash_n_n_mask(unsigned char * out, unsigned char const * in,
                  unsigned char const * mask)
{
	int i;
	unsigned char tmp[SPHINCS_BYTES];

	/* Mask input with XOR operation */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		tmp[i] = in[i] ^ mask[i];
	}

	return hash_n_n(out, tmp);
}

int hash_2n_n(unsigned char * out, unsigned char const * in)
{
	int i;
	u32 tmp[SPHINCS_BYTES/2];

	/* Translate the input in integers and expand with bytes from literal string */
	for(i = 0; i < SPHINCS_BYTES/4; ++i)
	{
		tmp[i] = U8TO32_LITTLE(in + 4*i);
		tmp[i + 8] = U8TO32_LITTLE(hash_const_c + 4*i);
	}

	/* Apply the permutation step of ChaCha12 */
	chacha12_perm_c(tmp, tmp);

	/* XOR the result with next SPHINCS_BYTES bytes of the input */
	for(i = 0; i < SPHINCS_BYTES/4; ++i)
	{
		tmp[i] = tmp[i] ^ U8TO32_LITTLE(in + 4*i + SPHINCS_BYTES);
	}

	/* Re-apply the permutation step of ChaCha12 */
	chacha12_perm_c(tmp, tmp);

	/* Translate the (SPHINCS_BYTES/4) first integers in bytes */
	for (i = 0; i < SPHINCS_BYTES/4; ++i)
	{
		U32TO8_LITTLE(out + 4*i, tmp[i]);
	}

	return 0;
}

int hash_2n_n_mask(unsigned char * out, unsigned char const * in,
                   unsigned char const * mask)
{
	int i;
	unsigned char tmp[2*SPHINCS_BYTES];

	/* Mask input with XOR operation */
	for (i = 0; i < 2*SPHINCS_BYTES; ++i)
	{
		tmp[i] = in[i] ^ mask[i];
	}

	return hash_2n_n(out, tmp);
}

int hash_nn_n(unsigned char * out, unsigned char const * in1,
              unsigned char const * in2)
{
	int i;
	unsigned char tmp[2*SPHINCS_BYTES];

	/* Concatenate inputs in parallel */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		tmp[i] = in1[i];
		tmp[i + SPHINCS_BYTES] = in2[i];
	}

	return hash_2n_n(out, tmp);
}

int hash_nn_n_mask(unsigned char * out, unsigned char const * in1,
                   unsigned char const * in2, unsigned char const * mask)
{
	int i;
	unsigned char tmp[2*SPHINCS_BYTES];

	/* Mask input with XOR operation while concatenating inputs in parallel */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		tmp[i] = in1[i] ^ mask[i];
		tmp[i + SPHINCS_BYTES] = in2[i] ^ mask[i + SPHINCS_BYTES];
	}

	return hash_2n_n(out, tmp);
}

int hash_chain_n(unsigned char * out, unsigned char const * in,
                 unsigned long const chainlen)
{
	unsigned int i;

	/* Initialize chain */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		out[i] = in[i];
	}

	/* Apply hash function in chain */
	for (i = 0; i < chainlen; ++i)
	{
		hash_n_n(out, out);
	}

	return 0;
}

int hash_chain_n_mask(unsigned char * out, unsigned char const * in,
                      unsigned char const * masks, unsigned long const chainlen)
{
	unsigned int i;

	/* Initialize chain */
	for (i = 0; i < SPHINCS_BYTES; ++i)
	{
		out[i] = in[i];
	}

	/* Apply hash function in chain and mask every step with XOR operation */
	for (i = 0; i < chainlen; ++i)
	{
		hash_n_n_mask(out, out, masks + (i*SPHINCS_BYTES));
	}

	return 0;
}
