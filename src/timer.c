/**
 * @file     timer.c
 * @author   Huo Huansong
 * @brief    Timer correlation function, without ISR.
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
 * @brief   Timer initialization function.
 * @param   none
 * @return  none
 */
void timer_init(void)
{
	timer0_init();
}

/**
 * @brief   Timer0 initialization function.
 * @param   none
 * @return  none
 */
void timer0_init(void)
{
	AUXR |= 0x80; /* set T0x12 bit */
	TMOD &= 0xF0; /* set T0 mode, timer mode & 16 bit auto reload */
	TL0 = 0xCD;   /* 1 ms */
	TH0 = 0xD4;
	TF0 = 0;      /* clear the interrupt identification bit */
}

/**
 * @brief     Timer0 enable o disable.
 * @param[in] en: ENABLE or DISABLE.
 * @return    none
 */
void timer0_set_enable(unsigned char en)
{
	TR0 = (en == ENABLE) ? 1 : 0;
}

/**
 * @brief     Timer1 enable o disable.
 * @param[in] en: ENABLE or DISABLE.
 * @return    none
 */
void timer1_set_enable(unsigned char en)
{
	TR1 = (en == ENABLE) ? 1 : 0;
}

/**
 * @brief     Timer0 start.
 * @param     none
 * @return    none
 */
void timer0_start(void)
{
	timer0_set_enable(ENABLE);
	SET_TIMER0_INTERRUPT_SWITCH(ENABLE);
}

/**
 * @brief     Timer0 stop.
 * @param     none
 * @return    none
 */
void timer0_stop(void)
{
	timer0_set_enable(DISABLE);
	SET_TIMER0_INTERRUPT_SWITCH(DISABLE);
}