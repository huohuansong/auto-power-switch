/**
 * @file     delay.h
 * @author   Huo Huansong
 * @brief    Delay correlation function header file.
 * @details
 * @date     2023-11-30
 */

#ifndef _DELAY_H_
#define _DELAY_H_

/* Macro & Enum */
enum delay_node_index {
	DELAY_NODE_HEARTBEAT_LED = 0,
	DELAY_NODE_PORT_RESET,
	DELAY_NODE_MAX
};

/* Structure */
struct delay_node_st {
	unsigned char valid: 1;
	unsigned char reload: 1;
	unsigned char arg;
	unsigned short reset_value;
	unsigned short current_value;
	void (*callback)(unsigned char arg);
};

/* Function declaration */
void delay_node_traverse(void);
void delay_init(void);

#endif