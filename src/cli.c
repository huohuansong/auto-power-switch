/**
 * @file     cli.c
 * @author   Huo Huansong
 * @brief    Cli function entry.
 * @details
 * @date     2023-08-20
 */

#include "STC8G.h"
#include "common.h"
#include "board.h"
#include "timer.h"
#include "interrupt.h"
#include "gpio.h"
#include "uart.h"
#include "cli.h"
#include "cli_cmd.h"

/* Global */
/* Module internal variable */
#if defined(TARGET_CHIP_STC8G1K17A)
__xdata static unsigned char cli_cmdline_buffer[CLI_CMDLINE_BUFFER_SIZE + 1] = {0};
#else
static unsigned char cli_cmdline_buffer[CLI_CMDLINE_BUFFER_SIZE + 1] = {0};
#endif
static volatile unsigned char cli_cmdline_buffer_length = 0;
static volatile unsigned char cli_cmdline_buffer_pos = 0;
static volatile unsigned char cli_cmdline_state = CLI_CMDLINE_STATE_INIT;

/**
 * @brief     CLI initialization function.
 * @param[in] none
 * @return    none
 */
void cli_init(void)
{
	unsigned int i = 0;

	for (i = 0; i < CLI_CMDLINE_BUFFER_SIZE; ++i) {
		cli_cmdline_buffer[i] = 0;
	}
	cli_cmdline_buffer_length = 0;
	cli_cmdline_buffer_pos = 0;

	almost_delay_ms(100);
	cli_print_string(CLI_WELCOME);
	cli_print_string(CLI_HELP);
	cli_print_prompt();

	cli_cmdline_state = CLI_CMDLINE_STATE_NORMAL;
}

/**
 * @brief     print prompts.
 * @param[in] none
 * @return    none
 */
void cli_print_prompt(void)
{
	cli_print_string(CLI_PROMPT);
}

/**
 * @brief     print one character to console by HEX style.
 * @param[in] data: the character that will be printed.
 * @return    none
 */
void cli_print_hex(const unsigned char data)
{
	unsigned char upper = (unsigned char)(data >> 4) & 0x0F;
	unsigned char lower = (unsigned char)(data) & 0x0F;

	uart1_send_byte('0');
	uart1_send_byte('x');
	uart1_send_byte((upper > 9) ? (upper - 10 + 'A') : (upper + '0'));
	uart1_send_byte((lower > 9) ? (lower - 10 + 'A') : (lower + '0'));
}

/**
 * @brief     print one character to console.
 * @param[in] data: the character that will be printed.
 * @return    none
 */
void cli_print_char(const unsigned char data)
{
	uart1_send_byte(data);
}

/**
 * @brief     print string to console.
 * @param[in] data: the character that will be printed.
 * @return    none
 */
void cli_print_string(unsigned char *str)
{
	unsigned char *ptr = NULL;

	if (NULL == str) {
		return;
	}

	ptr = str;
	while (*ptr) {
		uart1_send_byte(*ptr);
		ptr++;
	}
}

/**
 * @brief     A function that handles characters.
 * @param[in] data: the character that will be processed.
 * @return    none
 */
void cli_process_char(const unsigned char data)
{
	unsigned char i = 0;
	unsigned char retval = 0;

	switch (cli_cmdline_state) {
		case CLI_CMDLINE_STATE_INIT:
			break;
		case CLI_CMDLINE_STATE_NORMAL:
			/* normal character */
			if ((data >= CHR_SYMBOL_1_RANGE_MIN && data <= CHR_SYMBOL_1_RANGE_MAX)
			|| (data >= CHR_NUMBER_RANGE_MIN && data <= CHR_NUMBER_RANGE_MAX)
			|| (data >= CHR_SYMBOL_2_RANGE_MIN && data <= CHR_SYMBOL_2_RANGE_MAX)
			|| (data >= CHR_UPPERCASE_LETTER_RANGE_MIN && data <= CHR_UPPERCASE_LETTER_RANGE_MAX)
			|| (data >= CHR_SYMBOL_3_RANGE_MIN && data <= CHR_SYMBOL_3_RANGE_MAX)
			|| (data >= CHR_LOWERCASE_LETTER_RANGE_MIN && data <= CHR_LOWERCASE_LETTER_RANGE_MAX)
			|| (data >= CHR_SYMBOL_4_RANGE_MIN && data <= CHR_SYMBOL_4_RANGE_MAX)
			) {
				/* cmdline buffer is full. */
				if (cli_cmdline_buffer_pos == CLI_CMDLINE_BUFFER_SIZE) {
					/* do nothing */
				} else {
					cli_cmdline_buffer[cli_cmdline_buffer_pos++] = data;
					cli_print_char(data);
				}
			/* Enter */
			} else if (data == CHR_CARRIAGE_RETURN) {
				/* process this command */
				if (cli_cmdline_buffer_pos > 0) {
					retval = cli_cmd_parse(cli_cmdline_buffer);
					if (CLI_PROCESS_RET_SUCCESS != retval) {
						cli_print_string("^\r\n");
						for (i = 0; i < cli_cmdline_buffer_pos; ++i) {
							if ('\0' == cli_cmdline_buffer[i]) {
								cli_print_char(' ');
							} else {
								cli_print_char(cli_cmdline_buffer[i]);
							}
						}
						cli_print_string("\r\n");
						for (i = 0; i < retval; ++i) {
							cli_print_char(' ');
						}
						cli_print_string("^\r\n");
						for (i = 0; i < retval; ++i) {
							cli_print_char(' ');
						}
						cli_print_string("Error may be here, please check.\r\n");
					}

					/* reset 'cli_cmdline_buffer' and 'cli_cmdline_buffer_pos' */
					while (cli_cmdline_buffer_pos > 0) {
						cli_cmdline_buffer[cli_cmdline_buffer_pos--] = '\0';
					}
				}
				/* process done */
				cli_print_string("\r\n");
				cli_print_prompt();
			/* Backspace */
			} else if (data == CHR_BACKSPACE) {
				if (cli_cmdline_buffer_pos > 0) {
					cli_cmdline_buffer[--cli_cmdline_buffer_pos] = 0;
					cli_print_char('\b');
					cli_print_char(' ');
					cli_print_char('\b');
				}
			/* Direction key: left 'ESC [ D' and right 'ESC [ C', but do not support up and down. */
			} else if (data == CHR_ESCAPE) {
				cli_cmdline_state = CLI_CMDLINE_STATE_ESC;
			} else {
				/* do nothing */
			}
			break;
		case CLI_CMDLINE_STATE_BUSY:
			break;
		case CLI_CMDLINE_STATE_ESC:
			if ('[' == data) {
				cli_cmdline_state = CLI_CMDLINE_STATE_ESC_LEFT_BRACKET;
			} else {
				/* invalid */
				cli_cmdline_state = CLI_CMDLINE_STATE_NORMAL;
			}
			break;
		case CLI_CMDLINE_STATE_ESC_LEFT_BRACKET:
			if ('D' == data) {    /* 'ESC [ D' = left arrow */
				if (cli_cmdline_buffer_pos > 0) {
					/**/
				}
			} else if ('C' == data) {   /* 'ESC [ C' = right arrow */
					/**/
			} else {
				/* invalid */	
			}
			cli_print_string("HEHE\r\n");
			cli_cmdline_state = CLI_CMDLINE_STATE_NORMAL;
		case CLI_CMDLINE_STATE_STOP:
			break;
		default:
			break;
	}
}
