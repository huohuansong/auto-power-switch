/**
 * @file     interrupt.c
 * @author   Huo Huansong
 * @brief    Interrupt Service Routine.
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
#include "delay.h"

/* Global */
volatile __bit g_uart1_tx_busy = 0;

/**
 * @brief   Timer0 ISR
 * @param   none
 * @return  none
 */
void timer0_isr(void) __interrupt TF0_VECTOR
{
	delay_node_traverse();
}

/**
 * @brief   UART 1 ISR
 * @param   none
 * @return  none
 */
void uart1_isr(void) __interrupt SI0_VECTOR
{
	if (TI) {
		TI = 0;
		g_uart1_tx_busy = 0;
	}
	if (RI) {
		RI = 0;
		if (g_uart1_rx_buffer_count < UART_RECEIVE_BUFFER_SIZE) {
			g_uart1_rx_buffer[g_uart1_rx_buffer_in_ptr] = SBUF;
			if (g_uart1_rx_buffer_in_ptr == UART_RECEIVE_BUFFER_SIZE - 1) {
				g_uart1_rx_buffer_in_ptr = 0;
			} else {
				g_uart1_rx_buffer_in_ptr++;
			}
			g_uart1_rx_buffer_count++;
		}
	}
}