/**
 * @file     uart.h
 * @author   Huo Huansong
 * @brief    UART correlation function header file.
 * @details
 * @date     2023-08-19
 */

#ifndef _UART_H_
#define _UART_H_

/* Macro & Enum */
#define BAUD_RATE_9600             9600
#define BAUD_RATE_38400            38400
#define BAUD_RATE_115200           115200

#define UART1_BAUD_RATE            BAUD_RATE_115200
#define UART1_TH1_TL1_DEFAULT      (65536 - CPU_IRC_FREQUENCY / UART1_BAUD_RATE / 4)
#define UART1_TL1_DEFAULT          (unsigned char)(UART1_TH1_TL1_DEFAULT & 0xFF)
#define UART1_TH1_DEFAULT          (unsigned char)((UART1_TH1_TL1_DEFAULT >> 8) & 0xFF)

#define UART_RECEIVE_BUFFER_SIZE   16

#define SET_UART1_INTERRUPT_SWITCH(en) do { \
		ES = (ENABLE == en) ? 1 : 0; \
	} while(0)

/* External variable declaration */
extern unsigned char g_uart1_rx_buffer[UART_RECEIVE_BUFFER_SIZE];
extern unsigned char g_uart1_rx_buffer_count;
extern unsigned char g_uart1_rx_buffer_in_ptr;
extern unsigned char g_uart1_rx_buffer_out_ptr;

/* Function declaration */
void uart_init(void);
void uart1_set_enable(unsigned char en);
void uart1_start(void);
void uart1_stop(void);
void uart1_send_byte(unsigned char data);
unsigned char uart1_receive_byte(unsigned char *data);

#endif