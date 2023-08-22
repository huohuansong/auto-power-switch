/**
 * @file     board.h
 * @author   Huo Huansong
 * @brief    Functions related to a specific board header file.
 * @details
 * @date     2023-08-19
 */

#ifndef _BOARD_H_
#define _BOARD_H_

/* Macro & Enum */
#define _nop_() __asm NOP __endasm

#define CPU_IRC_FREQUENCY 11059200UL

#define LED_OFF 0
#define LED_ON  1

#define POWER_CHANNEL_1   1
#define POWER_CHANNEL_2   2
#define POWER_CHANNEL_NUM 2

#define POWER_OFF 0
#define POWER_ON  1

#define POWER_CONTROL_MODE_AUTO   0
#define POWER_CONTROL_MODE_MANUAL 1

#define SET_HEARTBEAT_LED_STATUS(onoff) do { \
		P5_5 = (LED_ON == onoff) ? 0 : 1; \
	} while(0)

#define SET_POWER_STATUS(channel, onoff) do { \
		if (POWER_CHANNEL_1 == channel) { \
			P3_2 = (POWER_ON == onoff) ? 1 : 0; \
		} else if (POWER_CHANNEL_2 == channel)  { \
			P3_3 = (POWER_ON == onoff) ? 1 : 0; \
		} else { \
			\
		} \
	} while(0)

/* Inline functions */
inline void almost_delay1ms(void)
{
	unsigned short i = 0;

	for (i = 0; i < 800; ++i) {
		_nop_();
	}
}

inline void almost_delay_ms(unsigned int n)
{
	unsigned int i = 0;

	for (i = 0; i < n; ++i) {
		almost_delay1ms();
	}
}

inline void almost_delay_s(unsigned int n)
{
	unsigned int i = 0;

	for (i = 0; i < n; ++i) {
		almost_delay_ms(1000);
	}
}

/* Function declaration */
void board_init(void);


#endif