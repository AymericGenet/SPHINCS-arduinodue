/*
 * minunit.h
 *
 * Source: http://www.jera.com/techinfo/jtns/jtn002.html
 */

#include <stdio.h>

extern int tests_run;

#define mu_assert(message, test) do { \
	if (!(test)) { \
		return message; \
	} \
} while (0)

#define mu_run_test(test, name) do { \
	test_head(name); \
	char const * message = test(); \
	tests_run++; \
	if (message) { \
		test_failure; \
		printf(" * %s\n", message); \
	} else { \
		test_success; \
	} \
} while (0)

#define test_head(name) printf("Test %s... ", name)
#define test_success printf("[OK]\n")
#define test_failure printf("[FAIL]\n")