/**
 * @file     main.c
 * @author   Huo Huansong
 * @brief    Main function entry.
 * @details
 * @date     2023-08-19
 */

#include "STC8G.h"
#include "common.h"
#include "board.h"
#include "timer.h"
#include "interrupt.h"
#include "gpio.h"
#include "uart.h"
#include "port.h"
#include "cli.h"
#include "cli_cmd.h"

/**
 * @brief   main function.
 * @param   none
 * @return  none
 */
void main(void)
{
	unsigned char data = 0;
	unsigned char retval = FALSE;

	/* Driver initialization */
	board_init();
	timer0_start();
	uart1_start();
	global_interrupt_enable();

	/* Application initialization */
	port_init();
	cli_cmd_init(cli_print_string);
	cli_init();

	/* Main process */
	while(1) {
		retval = uart1_receive_byte(&data);
		if (TRUE == retval) {
			cli_process_char(data);
		} else {
			almost_delay_ms(10);
		}
	}
}
