/*
 * test_hash.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "utils.h"
#include "hash.h"
#include "test_hash.h"

static char const * test_hash_any_n(void)
{
	unsigned char out[SPHINCS_BYTES];

	/* in1 test case */
	hash_any_n(out, in1, IN1_LEN);

	mu_assert("Test case failed for IN1.", array_cmp(out, in1_hashed_256, SPHINCS_BYTES));

	/* in2 test case */
	hash_any_n(out, in2, IN2_LEN);

	mu_assert("Test case failed for IN2.", array_cmp(out, in2_hashed_256, SPHINCS_BYTES));

	return NULL; /* Pass */
}

static char const * test_hash_n_n(void)
{
	unsigned char out[SPHINCS_BYTES];

	/* Expected output for in1_hashed_256 */
	unsigned char const exp1[SPHINCS_BYTES] = {
		0x4a, 0xcd, 0x1f, 0x30, 0xdc, 0x33, 0xfe, 0xd2,
		0xa9, 0x90, 0x48, 0x23, 0x41, 0x93, 0x3a, 0x98,
		0x59, 0x89, 0xf0, 0x1c, 0x92, 0x78, 0x9a, 0xb9,
		0x20, 0x58, 0x2e, 0x39, 0xdf, 0x87, 0xd0, 0xe8
	};
	/* Expected output for in2_hashed_256 */
	unsigned char const exp2[SPHINCS_BYTES] = {
		0x39, 0x45, 0x9e, 0xa7, 0xfd, 0x5f, 0xc1, 0xfa,
		0xec, 0xb8, 0x76, 0xe8, 0x73, 0xf0, 0xcd, 0x34,
		0xe1, 0x54, 0xba, 0x70, 0x50, 0xb1, 0x61, 0x99,
		0x4c, 0x11, 0x88, 0xe7, 0x3d, 0xa1, 0xcf, 0xfe
	};

	/* in1_hashed_256 test case */
	hash_n_n(out, in1_hashed_256);

	mu_assert("Test case failed for IN1.", array_cmp(out, exp1, SPHINCS_BYTES));

	/* in2_hashed_256 test case */
	hash_n_n(out, in2_hashed_256);

	mu_assert("Test case failed for IN2.", array_cmp(out, exp2, SPHINCS_BYTES));

	return NULL; /* Pass */
}

static char const * test_hash_n_n_mask(void)
{
	unsigned char out[SPHINCS_BYTES];

	/* Expected output for in1_hashed_256 masked with in2_hashed_256 */
	unsigned char const exp[SPHINCS_BYTES] = {
		0xb5, 0xb8, 0xa1, 0x51, 0x13, 0xcb, 0xb8, 0x81,
		0x3d, 0x8e, 0x16, 0x36, 0x2c, 0x1d, 0x87, 0xfe,
		0x63, 0x4f, 0x9e, 0x75, 0xa6, 0x3b, 0x7e, 0x37,
		0x10, 0x1e, 0xf1, 0x3c, 0xc0, 0xf9, 0x2c, 0xc0
	};

	/* in1_hashed_256 masked with in2_hashed_256 test case */
	hash_n_n_mask(out, in1_hashed_256, in2_hashed_256);

	mu_assert("Test case failed for IN1 masked with IN2.", array_cmp(out, exp, SPHINCS_BYTES));

	return NULL; /* Pass */
}

static char const * test_hash_2n_n(void)
{
	unsigned char out[SPHINCS_BYTES];

	/* Expected output for in1_hashed_512 */
	unsigned char const exp1[SPHINCS_BYTES] = {
		0x5b, 0x55, 0x2f, 0xba, 0x92, 0x98, 0x96, 0xda,
		0x50, 0x05, 0x9d, 0x0f, 0x7c, 0x0d, 0x87, 0xce,
		0xd0, 0x46, 0xca, 0xbb, 0x8d, 0x05, 0x45, 0xa7,
		0x28, 0xcc, 0x47, 0x5f, 0x3b, 0xb7, 0x36, 0x76
	};

	/* Expected output for in2_hashed_512 */
	unsigned char const exp2[SPHINCS_BYTES] = {
		0xff, 0x31, 0xfc, 0xb7, 0xa6, 0xc8, 0x70, 0x38,
		0xb4, 0x10, 0xb3, 0x04, 0xa0, 0xde, 0xcf, 0x8b,
		0xbe, 0x4d, 0x4e, 0x7b, 0x9f, 0x01, 0x98, 0x0b,
		0x88, 0x5f, 0xeb, 0x9f, 0x99, 0xe9, 0x13, 0xf5
	};

	/* in1_hashed_512 test case */
	hash_2n_n(out, in1_hashed_512);

	mu_assert("Test case failed for IN1.", array_cmp(out, exp1, SPHINCS_BYTES));

	/* in2_hashed_512 test case */
	hash_2n_n(out, in2_hashed_512);

	mu_assert("Test case failed for IN2.", array_cmp(out, exp2, SPHINCS_BYTES));

	return NULL; /* Pass */
}

static char const * test_hash_2n_n_mask(void)
{
	unsigned char out[SPHINCS_BYTES];

	/* Expected output for in1_hashed_512 masked with in2_hashed_512 */
	unsigned char const exp[SPHINCS_BYTES] = {
		0x46, 0xfb, 0xb7, 0x4d, 0x15, 0xa8, 0xfa, 0xb3,
		0xd3, 0xda, 0xe2, 0x54, 0xba, 0x88, 0x45, 0x5c,
		0xf6, 0x65, 0xcd, 0x12, 0x0d, 0xde, 0xcd, 0xd6,
		0x34, 0xf6, 0x8c, 0x34, 0xf6, 0x73, 0x59, 0xe3
	};

	/* in1_hashed_512 test case */
	hash_2n_n_mask(out, in1_hashed_512, in2_hashed_512);

	mu_assert("Test case failed for IN1 masked with IN2.", array_cmp(out, exp, SPHINCS_BYTES));

	return NULL; /* Pass */
}

static char const * test_hash_nn_n(void)
{
	unsigned char out[SPHINCS_BYTES];

	/* Expected output for in1_hashed_512 masked with in2_hashed_512 */
	unsigned char const exp[SPHINCS_BYTES] = {
		0x42, 0x17, 0x3b, 0xb4, 0x1e, 0x1d, 0x2b, 0x06,
		0x53, 0xc1, 0xa8, 0xce, 0x6e, 0x8e, 0x5a, 0xd5,
		0x21, 0xa2, 0x15, 0x02, 0x54, 0xd6, 0x21, 0xf8,
		0xbb, 0xed, 0x4c, 0x96, 0xad, 0x4f, 0x74, 0xd5
	};

	/* Expected output for in1_hashed_256 + in2_hashed_256 */
	hash_nn_n(out, in1_hashed_256, in2_hashed_256);

	mu_assert("Test case failed for IN1 + IN2.", array_cmp(out, exp, SPHINCS_BYTES));

	return NULL; /* Pass */
}

void run_test_hash(void)
{
	printf("### TEST_HASH ###\n\n");

	mu_run_test(test_hash_any_n, "test_hash_any_n()");
	mu_run_test(test_hash_n_n, "test_hash_n_n()");
	mu_run_test(test_hash_n_n_mask, "test_hash_n_n_mask()");
	mu_run_test(test_hash_2n_n, "test_hash_2n_n()");
	mu_run_test(test_hash_2n_n_mask, "test_hash_2n_n_mask()");
	mu_run_test(test_hash_nn_n, "test_hash_nn_n()");

	printf("\n");
}
