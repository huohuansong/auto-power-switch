/**
 * @file     uart.c
 * @author   Huo Huansong
 * @brief    UART correlation function, without ISR.
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

/* Global */
unsigned char g_uart1_rx_buffer[UART_RECEIVE_BUFFER_SIZE] = {0};
unsigned char g_uart1_rx_buffer_count = 0;
unsigned char g_uart1_rx_buffer_in_ptr = 0;
unsigned char g_uart1_rx_buffer_out_ptr = 0;

/**
 * @brief   UART initialization function.
 * @param   none
 * @return  none
 */
void uart_init(void)
{
	SCON = 0x50;               /* mode 1: variable baud rate, 8-bit data mode. */
	AUXR |= 0x40;              /* set T1x12 bit */
	AUXR &= 0xFE;              /* use T1 as the baud rate generator. */
	TMOD &= 0x0F;              /* set timer mode. */
	TL1 = UART1_TL1_DEFAULT;   /* set initial value */
	TH1 = UART1_TH1_DEFAULT;
	SET_TIMER1_INTERRUPT_SWITCH(DISABLE); /* disable timer1 interrupt */
}

/**
 * @brief     UART1 enable or disable.
 * @param[in] en: ENABLE or DISABLE.
 * @return    none
 */
void uart1_set_enable(unsigned char en)
{
	timer1_set_enable(en);
	return;
}

/**
 * @brief     UART 1 start.
 * @param     none
 * @return    none
 */
void uart1_start(void)
{
	uart1_set_enable(ENABLE);
	SET_UART1_INTERRUPT_SWITCH(ENABLE);
}

/**
 * @brief     UART 1 stop.
 * @param     none
 * @return    none
 */
void uart1_stop(void)
{
	uart1_set_enable(DISABLE);
	SET_UART1_INTERRUPT_SWITCH(DISABLE);
}

/**
 * @brief     UART1 send one byte
 * @param[in] data: the data to be send.
 * @return    none
 */
void uart1_send_byte(unsigned char data)
{
	while(g_uart1_tx_busy);
	g_uart1_tx_busy = 1;
	SBUF = data;
}

/**
 * @brief      get one byte from UART 1 receive buffer.
 * @param[out] data: the data received.
 * @return     unsigned char: weather get successful or not.
 */
unsigned char uart1_receive_byte(unsigned char *data)
{
	if (g_uart1_rx_buffer_count > 0) {
		*data = g_uart1_rx_buffer[g_uart1_rx_buffer_out_ptr];
		if (g_uart1_rx_buffer_out_ptr == UART_RECEIVE_BUFFER_SIZE - 1) {
			g_uart1_rx_buffer_out_ptr = 0;
		} else {
			g_uart1_rx_buffer_out_ptr++;
		}
		g_uart1_rx_buffer_count--;
		return TRUE;
	}

	return FALSE;
}