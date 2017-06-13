/*
 * prf.c
 *
 */

#include "prf.h"
#include "BLAKE-256.h"
#include "BLAKE-512.h"
#include "parameters.h"

int prf_64(unsigned char out[SPHINCS_BYTES],
           unsigned char const in1[SPHINCS_ADDRESS_BYTES],
           unsigned char const in2[SEED_BYTES])
{
	blake256_state S;
	blake256_init(&S);
	blake256_update(&S, in2, SEED_BYTES*8);
	blake256_update(&S, in1, SPHINCS_ADDRESS_BYTES*8);
	blake256_final(&S, out);
	return 0;
}

int prf(unsigned char out[2*SPHINCS_BYTES],
        unsigned char const * in1,
        unsigned long long in1len,
        unsigned char const in2[SEED_BYTES])
{
	blake512_state S;
	blake512_init(&S);
	blake512_update(&S, in2, SEED_BYTES*8);
	if (in1len > 0)
	{
		blake512_update(&S, in1, in1len*8);
	}
	blake512_final(&S, out);
	return 0;
}
