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
	/* keygen with in_sk and in_masks */
	printf("\nHORST root (this makes take a few seconds...):\t");
	horst_keygen(in_sk1, in_masks);

	return NULL; /* Skip */
}


void run_test_horst(void)
{
	printf("### TEST_HORST ###\n\n");

	mu_run_test(test_horst_keygen, "test_horst_keygen()");

	printf("\n");
}
