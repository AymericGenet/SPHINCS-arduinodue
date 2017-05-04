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
	printf("Test %s... ", name); \
	char const * message = test(); \
	tests_run++; \
	if (message) { \
		if (message[0] != '\0') { \
			printf("[FAIL]\n * %s\n", message); \
		} else { \
			printf("[OK]\n"); \
		} \
	} else { \
		printf("\n"); \
	} \
} while (0)
