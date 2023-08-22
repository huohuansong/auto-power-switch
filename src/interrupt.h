/**
 * @file     interrupt.h
 * @author   Huo Huansong
 * @brief    Interrupt Service Routine header file.
 * @details
 * @date     2023-08-19
 */

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

/* Macro & Enum */

/* External variable declaration */
extern volatile __bit g_uart1_tx_busy;

/* Inline functions */
inline void global_interrupt_enable(void)
{
	EA = 1;
}
inline void global_interrupt_disable(void)
{
	EA = 0;
}

/* Function declaration */
/* MUST declare */
void timer0_isr(void) __interrupt TF0_VECTOR;
void uart1_isr(void) __interrupt SI0_VECTOR;

#endif