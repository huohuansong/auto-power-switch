/**
 * @file     gpio.c
 * @author   Huo Huansong
 * @brief    GPIO correlation function, without ISR.
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

/**
 * @brief   GPIO initialization function.
 * @param   none
 * @return  none
 */
void gpio_init(void)
{
	P3_2 = 1;
	P3_3 = 1;
	P5_4 = 1;
	P3M0 = 0;
	P3M1 = 0;
	P5M0 = 0;
	P5M1 = 0;
}
