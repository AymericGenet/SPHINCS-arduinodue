/*
 * test_prng.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "test_prng.h"
#include "prng.h"
#include "ecrypt-sync.h"
#include "utils.h"

static char const * test_prng(void)
{
	unsigned char out[128];

	/* Call 1 byte */
	prng(out, 1, in_sk1);
	mu_assert("Test case failed for PRNG call of 1 byte.", array_cmp(out, prng_stream, 1));

	/* Call 32 bytes */
	prng(out, 32, in_sk1);
	mu_assert("Test case failed for PRNG call of 32 bytes.", array_cmp(out, prng_stream, 32));

	/* Call 64 bytes */
	prng(out, 64, in_sk1);
	mu_assert("Test case failed for PRNG call of 64 bytes.", array_cmp(out, prng_stream, 64));

	/* Call 128 bytes */
	prng(out, 128, in_sk1);
	mu_assert("Test case failed for PRNG call of 128 bytes.", array_cmp(out, prng_stream, 128));

	return ""; /* Success */
}

static char const * test_prng_next(void)
{
	ECRYPT_ctx ctx;
	unsigned char out[PRNG_BYTES];

	/* Setup */
	prng_context_setup(&ctx, in_sk1);

	/* Call 64 first bytes */
	prng_next(&ctx, out);
	mu_assert("Test case failed for first PRNG call.", array_cmp(out, prng_stream, PRNG_BYTES));

	/* Call 64 next bytes */
	prng_next(&ctx, out);
	mu_assert("Test case failed for next PRNG call.", array_cmp(out, prng_stream + PRNG_BYTES, PRNG_BYTES));

	/* Delete */
	prng_context_delete(&ctx);
	mu_assert("The context does not delete properly.", array_is_zero(ctx.input, 16));

	return "";
}


void run_test_prng(void)
{
	printf("### TEST_PRNG ###\n\n");

	mu_run_test(test_prng, "test_prng()");
	mu_run_test(test_prng_next, "test_prng_next()");

	printf("\n");
}
