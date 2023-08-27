/**
 * @file     port.h
 * @author   Huo Huansong
 * @brief    Functions related to port header file.
 * @details
 * @date     2023-08-27
 */

#ifndef _PORT_H_
#define _PORT_H_

/* Macro & Enum */
#define PORT_NUM 2
#define PORT_PHYSICAL_PORT_1   0
#define PORT_PHYSICAL_PORT_2   1

#define GET_UESR_PORT_FROM_PHYSICAL_PORT(phy_port) (phy_port + 1)
#define GET_PHYSICAL_PORT_FROM_USER_PORT(user_port) (user_port - 1)

#define PORT_POWER_OFF 0
#define PORT_POWER_ON  1

#define PORT_POWER_CONTROL_MODE_AUTO   0
#define PORT_POWER_CONTROL_MODE_MANUAL 1


#define SET_POWER_STATUS(phy_port, onoff) do { \
		if (PORT_PHYSICAL_PORT_1 == phy_port) { \
			P3_2 = (PORT_POWER_ON == onoff) ? 1 : 0; \
		} else if (PORT_PHYSICAL_PORT_2 == phy_port)  { \
			P3_3 = (PORT_POWER_ON == onoff) ? 1 : 0; \
		} else { \
			\
		} \
	} while(0)

/* Structure */
struct port_attr {
	unsigned char mode;
	unsigned char config_status;   /* only for manual mode */
	unsigned char running_status;
	unsigned char bind_profile;
};

/* Inline functions */


/* Function declaration */
void port_init(void);
unsigned char port_set_power_status(unsigned char phy_port, unsigned char status);
void port_get_port_status(unsigned char phy_port, struct port_attr *attr);

#endif