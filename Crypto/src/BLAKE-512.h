#include "defs.h"

#ifndef BLAKE512_H
#define BLAKE512_H

typedef struct  {
	u64 h[8], s[4], t[2];
	int buflen, nullt;
	u8 buf[128];
} blake512_state;

void blake512_init( blake512_state * S );
void blake512_update( blake512_state * S, const u8 * data, u64 datalen );
void blake512_final( blake512_state * S, u8 * digest );
int crypto_hash_blake512( unsigned char *out, const unsigned char *in, unsigned long long inlen );

#endif
