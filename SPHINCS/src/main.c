/**
 * \mainpage SPHINCS
 *
 * \section Purpose
 *
 * The SPHINCS-arduino project implements the stateless hash-based signature
 * scheme SPHINCS-256 for an Arduino Due board.
 *
 * \section Requirements
 *
 * This whole tutorial: http://www.elecrom.com/program-arduino-due-atmel-studio/
 *
 * \section Description
 *
 * The application generates a SPHINCS key pair and signs a string message
 * according to the SPHINCS procedure.
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
 * -# Wait for the program to generate key pair
 * -# Send a message to be signed through the terminal application
 *
 */

#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "conf_clock.h"

#include "BLAKE-256.h"

#include "parameters.h"

#define STRING_EOL    "\r"
#define STRING_HEADER "-- SPHINCS --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

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

/**
 *  \brief getting-started Application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */

int main(void)
{
	char string[257];
	u8 data[256], digest[32];
	int i, len;

	/* Initialize the SAM system */
	sysclk_init();
	board_init();

	/* Initialize the console UART */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	while (1) {
		/* Sets buffers to 0 */
		for (i = 0; i < 256; ++i) {
			string[i] = '\0';
			data[i] = 0;
			if (i < 32) {
				digest[i] = 0;
			}
		}
		string[256] = '\0';

		puts("Please send a message...");
		fgets(string, 256, stdin);

		len = 0;
		puts("\nParsing message...\n");
		for (i = 0; i < 256 && (string[i] != '\0' && string[i] != '\n'); ++i) {
			data[i] = (unsigned char) string[i];
			printf("%02x", data[i]);
			++len;
		}
		printf("\nReceived (len = %i) : %s \n", len, string);

		puts("Computing hash...\n");
		crypto_hash_blake256(digest, data, len);

		puts("Hashed : ");
		for (i = 0; i < 32; ++i) {
			printf("%02x", digest[i]);
		}
		printf("\n\n");
	}
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
