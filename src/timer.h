/**
 * @file     timer.h
 * @author   Huo Huansong
 * @brief    Timer correlation function header file.
 * @details
 * @date     2023-08-19
 */

#ifndef _TIMER_H_
#define _TIMER_H_

/* Macro & Enum */
#define SET_TIMER0_INTERRUPT_SWITCH(en) do { \
		ET0 = (ENABLE == en) ? 1 : 0; \
	} while(0)
#define SET_TIMER1_INTERRUPT_SWITCH(en) do { \
		ET1 = (ENABLE == en) ? 1 : 0; \
	} while(0)

/* Function declaration */
void timer_init(void);
void timer0_init(void);
void timer0_set_enable(unsigned char en);
void timer1_set_enable(unsigned char en);
void timer0_start(void);
void timer0_stop(void);

#endif