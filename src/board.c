/**
 * @file     board.c
 * @author   Huo Huansong
 * @brief    Functions related to a specific board.
 * @details  Use an internal oscillator with a frequency of 11.0592MHz, That is, IRC = 11.0592MHz.
 * @date     2023-08-19
 */

#include "STC8G.h"
#include "common.h"
#include "board.h"
#include "timer.h"
#include "interrupt.h"
#include "gpio.h"
#include "uart.h"

/**
 * @brief   Board initialization function.
 * @param   none
 * @return  none
 */
void board_init(void)
{
	almost_delay_ms(100);

	gpio_init();
	timer_init();
	uart_init();
}
