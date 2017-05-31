/*
 * crypto_chacha12.h
 *
 */ 

#include "defs.h"

#ifndef CRYPTO_CHACHA12_H_
#define CRYPTO_CHACHA12_H_

#define CHACHA12_KEY_BYTES 32
#define CHACHA12_NONCE_BYTES 8

void chacha12_perm_c(u32 x[16], const u32 input[16]);

#endif /* CRYPTO_CHACHA12_H_ */
