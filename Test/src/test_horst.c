/*
 * test_horst.c
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"
#include "test_horst.h"
#include "horst.h"
#include "utils.h"

static char const * test_horst_keygen(void)
{
	/* keygen with in_sk1 and in_masks */
	printf("(this may take a few seconds)\n");
	horst_keygen(in_sk1, in_masks);

	return NULL; /* Skip */
}

static char const * test_horst_sign(void)
{
	/* sign with in_sk1, in_masks, and in1_hashed_512 */
	printf("(this may take a few seconds)\n");
	horst_sign(in1_hashed_512, in_sk1, in_masks);

	return NULL;
}


void run_test_horst(void)
{
	printf("### TEST_HORST ###\n\n");

	mu_run_test(test_horst_keygen, "test_horst_keygen()");
	mu_run_test(test_horst_sign, "test_horst_sign()");

	printf("\n");
}
