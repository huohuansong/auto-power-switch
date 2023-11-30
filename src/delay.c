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

#include "port.h"

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
 * @brief   Callback function of port reset delay node.
 * @param   none
 * @return  none
 */
static void delay_callback_port_reset(unsigned char arg)
{
	unsigned char i = 0;

	for (i = PORT_PHYSICAL_PORT_1; i < PORT_NUM; ++i) {
		if (arg & (0x01 << i)) {
			port_set_power_status(i, PORT_POWER_ON);
		}
	}
}

/**
 * @brief   Traverse all delay node.
 * @param   none
 * @return  none
 */
int delay_node_set_valid(unsigned char index, unsigned char valid, unsigned char arg, unsigned short reset_value, unsigned char reload)
{
	if ((index < DELAY_NODE_HEARTBEAT_LED) || (index >= DELAY_NODE_MAX)) {
		return RET_PARA_ERROR;
	}

	g_delay_node[index].arg = arg;
	g_delay_node[index].current_value = reset_value;
	g_delay_node[index].reset_value = reset_value;
	g_delay_node[index].reload = reload;
	g_delay_node[index].valid = valid;
	return RET_SUCCESSFUL;
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
	g_delay_node[DELAY_NODE_HEARTBEAT_LED].valid = TRUE;
	g_delay_node[DELAY_NODE_HEARTBEAT_LED].reload = TRUE;
	g_delay_node[DELAY_NODE_HEARTBEAT_LED].reset_value = 1000;
	g_delay_node[DELAY_NODE_HEARTBEAT_LED].current_value = 1000;
	g_delay_node[DELAY_NODE_HEARTBEAT_LED].callback = delay_callback_heartbeat_led;

	g_delay_node[DELAY_NODE_PORT_RESET].valid = FALSE;
	g_delay_node[DELAY_NODE_PORT_RESET].callback = delay_callback_port_reset;
}
