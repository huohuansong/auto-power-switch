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

/* Global */
volatile __bit g_uart1_tx_busy = 0;
volatile unsigned char heartbeat_led_status = LED_OFF;
volatile unsigned short count = 0;

/**
 * @brief   Timer0 ISR
 * @param   none
 * @return  none
 */
void timer0_isr(void) __interrupt TF0_VECTOR
{
	count++;
	if (count == 1000) {
		count = 0;
		if (LED_ON == heartbeat_led_status) {
			heartbeat_led_status = LED_OFF;
		} else {
			heartbeat_led_status = LED_ON;
		}
		SET_HEARTBEAT_LED_STATUS(heartbeat_led_status);
		count = 0;
	}
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