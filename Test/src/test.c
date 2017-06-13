/**
 * \mainpage SPHINCS test project
 *
 * \section Purpose
 *
 * The SPHINCS-arduino test project is a suite of unit and integration tests
 * of the current implementation of SPHINCS on Arduino Due.
 *
 * \section Requirements
 *
 * This whole tutorial: http://www.elecrom.com/program-arduino-due-atmel-studio/
 *
 * \section Description
 *
 * The application goal is to test every function provided by the SPHINCS
 * implementation. The unit tests cover independent behavior and serve as
 * sanity checks, while integration tests cover the combination of the
 * functions together and communicates with a reference script.
 *
 * \section Usage
 *
 * -# Make sure to have completed the Requirements
 * -# Build the solution (F7)
 * -# Hold the ERASE button on the Arduino Due board for 2 seconds
 * -# Press the RESET button on the Arduino Due board once
 * -# Download the code on the board (Tools > Due Programmer)
 * -# Open and configure a terminal application with these settings:
 *   - 115200 bauds
 *   - 8 bits of data
 *   - No parity
 *   - 1 stop bit
 *   - No flow control
 * -# Start the application
 * -# The program runs many tests and displays their success on the terminal
 * -# It can start over by pressing the RESET button
 *
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

#include "minunit.h"
#include "parameters.h"

#include "test_utils.h"
#include "test_hash.h"
#include "test_prng.h"
#include "test_prf.h"
#include "test_wotsp.h"
#include "test_trees.h"
#include "test_sphincs.h"
#include "test_horst.h"

#define STRING_EOL    "\r"
#define STRING_HEADER "-- SPHINCS Test Project --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL \
		"\n"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/**
 *  Configure UART console.
 */

static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/* Test functions to execute */
#define SUITES_AMOUNT 8

void (*tests[SUITES_AMOUNT])(void) = {
	run_test_utils,
	run_test_hash,
	run_test_prng,
	run_test_prf,
	run_test_wotsp,
	run_test_trees,
	run_test_horst,
	run_test_sphincs
};

/* Total tests run */
int tests_run = 0;

/**
 *  \brief getting-started Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */

int main(void)
{
	char input[SPHINCS_BYTES];
	int i;

	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Initialize the console UART */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Run all the unit tests */
	for (i = 0; i < SUITES_AMOUNT; ++i)
	{
		(*tests[i])();
	}

	/* Summary */
	puts("=============================");
	printf("Total number of tests run: %d\n", tests_run);

	/* Run integrity tests */
	while (1)
	{
		/* Wait for input */
		fgets(input, SPHINCS_BYTES, stdin);
		printf("%s\n", input);
	}
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
