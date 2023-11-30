/**
 * @file     delay.c
 * @author   Huo Huansong
 * @brief    Delay correlation function.
 * @details
 * @date     2023-11-30
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
#if defined(TARGET_CHIP_STC8G1K17A)
__xdata static struct delay_node_st g_delay_node[DELAY_NODE_MAX];
#else
static struct delay_node_st g_delay_node[DELAY_NODE_MAX];
#endif

/**
 * @brief   Callback function of heartbeat led delay node.
 * @param   none
 * @return  none
 */
static void delay_callback_heartbeat_led(unsigned char arg)
{
#if defined(TARGET_CHIP_STC8G1K17A)
	__xdata static unsigned char heartbeat_led_status = LED_OFF;
#else
	static unsigned char heartbeat_led_status = LED_OFF;
#endif
	if (LED_ON == heartbeat_led_status) {
		heartbeat_led_status = LED_OFF;
	} else {
		heartbeat_led_status = LED_ON;
	}
	SET_HEARTBEAT_LED_STATUS(heartbeat_led_status);
}

/**
 * @brief   Traverse all delay node.
 * @param   none
 * @return  none
 */
void delay_node_traverse(void)
{
	unsigned char i = 0;

	for (i = DELAY_NODE_HEARTBEAT_LED; i < DELAY_NODE_MAX; ++i) {
		if (FALSE == g_delay_node[i].valid) {
			continue;
		}
		if (0 == g_delay_node[i].current_value) {
			g_delay_node[i].callback(g_delay_node[i].arg);
			if (TRUE == g_delay_node[i].reload) {
				g_delay_node[i].current_value = g_delay_node[i].reset_value;
			} else {
				g_delay_node[i].valid = FALSE;
			}
		} else {
			g_delay_node[i].current_value--;
		}
	}
}

/**
 * @brief   Delay initialization function.
 * @param   none
 * @return  none
 */
void delay_init(void)
{
	unsigned char i = 0;

	for (i = DELAY_NODE_HEARTBEAT_LED; i < DELAY_NODE_MAX; ++i) {
		g_delay_node[i].valid = FALSE;
	}

	g_delay_node[DELAY_NODE_HEARTBEAT_LED].valid = TRUE;
	g_delay_node[DELAY_NODE_HEARTBEAT_LED].reload = TRUE;
	g_delay_node[DELAY_NODE_HEARTBEAT_LED].reset_value = 1000;
	g_delay_node[DELAY_NODE_HEARTBEAT_LED].current_value = 1000;
	g_delay_node[DELAY_NODE_HEARTBEAT_LED].callback = delay_callback_heartbeat_led;
}
