/*
 * test_sphincs.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "test_sphincs.h"
#include "sphincs.h"
#include "utils.h"

static unsigned char const exp_y[SPHINCS_BYTES] = {
	0xd5, 0x00, 0x74, 0x68, 0xde, 0xbd, 0x41, 0x86,
	0x7c, 0x12, 0x4a, 0x0d, 0xe4, 0x5f, 0x9d, 0x56,
	0x35, 0x5a, 0xf6, 0x28, 0xa9, 0x8d, 0x50, 0x08,
	0x36, 0xa8, 0x05, 0x75, 0x12, 0xdc, 0x52, 0xce
};

static char const * test_sphincs_keygen(void)
{
	unsigned char y[SPHINCS_BYTES];

	/* Generate public key with in_sk and in_masks */
	sphincs_keygen(y, in_sk, in_masks);

	mu_assert("Public key invalid.", array_cmp(y, exp_y, SPHINCS_BYTES));

	return ""; /* Success */
}


void run_test_sphincs(void)
{
	printf("### TEST_SPHINCS ###\n\n");

	mu_run_test(test_sphincs_keygen, "test_sphincs_keygen()");

	printf("\n");
}
