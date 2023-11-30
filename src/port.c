/**
 * @file     port.c
 * @author   Huo Huansong
 * @brief    Functions related to port.
 * @details
 * @date     2023-08-27
 */

#include "STC8G.h"
#include "common.h"
#include "board.h"
#include "timer.h"
#include "interrupt.h"
#include "port.h"
#include "gpio.h"
#include "uart.h"

/* Global */
#if defined(TARGET_CHIP_STC8G1K17A)
__xdata static struct port_attr g_port[PORT_NUM];
#else
static struct port_attr g_port[PORT_NUM];
#endif

/**
 * @brief   Port initialization function.
 * @param   none
 * @return  none
 */
void port_init(void)
{
	unsigned char i = 0;

	for (i = 0; i < PORT_NUM; ++i) {
		SET_POWER_STATUS(i, PORT_POWER_OFF);
		g_port[i].mode = PORT_POWER_CONTROL_MODE_MANUAL;
		g_port[i].config_status = PORT_POWER_OFF;
		g_port[i].running_status = PORT_POWER_OFF;
		g_port[i].bind_profile = 255;
	}
}

/**
 * @brief   Set port power on or power off (manual mode).
 * @param[in] port: physical port
 * @param[in] status: power on or power off
 * @return  unsigned char: set successful or failed.
 */
unsigned char port_set_power_status(unsigned char phy_port, unsigned char status)
{
	unsigned char i = 0;

	if (phy_port >= PORT_NUM) {
		return RET_PARA_ERROR;
	}
	if (PORT_POWER_ON != status && PORT_POWER_OFF != status) {
		return RET_PARA_ERROR;
	}
	SET_POWER_STATUS(phy_port, status);
	g_port[phy_port].mode = PORT_POWER_CONTROL_MODE_MANUAL;
	g_port[phy_port].config_status = status;
	g_port[phy_port].running_status = status;

	return RET_SUCCESSFUL;
}

/**
 * @brief   Get port current status.
 * @param[in]  port: physical port
 * @param[out] attr: port status
 * @return  none
 */
void port_get_port_status(unsigned char phy_port, struct port_attr *attr)
{
	if (NULL == attr) {
		return;
	}

	attr->mode = g_port[phy_port].mode;
	attr->config_status = g_port[phy_port].config_status;
	attr->running_status = g_port[phy_port].running_status;
	attr->bind_profile = g_port[phy_port].bind_profile;
}
