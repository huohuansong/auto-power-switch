/**
 * @file     cli_cmd.c
 * @author   Huo Huansong
 * @brief    Cli command function entry.
 * @details
 * @date     2023-08-20
 */

#include "STC8G.h"
#include "common.h"
#include "board.h"
#include "timer.h"
#include "interrupt.h"
#include "gpio.h"
#include "uart.h"
#include "cli.h"
#include "cli_cmd.h"

#include "lib/string.h"
#include "lib/stdlib.h"

/* Global */
static void (*print_str)(unsigned char *str) = NULL;
static g_port_status[POWER_CHANNEL_NUM] = {0};
static g_port_mode[POWER_CHANNEL_NUM] = {0};

/**
 * @brief     CLI command initialization function.
 * @param[in] none
 * @return    none
 */
void cli_cmd_init(void *cb_pr_str)
{
	unsigned char i = 0;

	print_str = cb_pr_str;

	for (i = 0; i < POWER_CHANNEL_NUM; ++i) {
		g_port_status[i] = POWER_OFF;
		g_port_mode[i] = POWER_CONTROL_MODE_MANUAL;
	}
}

/**
 * @brief     CLI command parse function.
 * @param[in] cmd: command need to be handled.
 * @return    unsigned char:
 *              255   is successful;
 *              0~254 indicates the position of error command;
 */
unsigned char cli_cmd_parse(unsigned char *cmd)
{
	unsigned char argc = 0;
	unsigned char *argv[ARGC_MAX];// = {(unsigned char *)0};
	unsigned char argv_idx = 0;
	unsigned char *p = cmd;
	unsigned char state = CLI_PARSE_STATE_INIT;
	unsigned char i = 0;

	for (i = 0; i < ARGC_MAX; ++i) {
		argv[i] = NULL;
	}

	while (*p) {
		switch (state) {
			case CLI_PARSE_STATE_INIT:
				if (' ' == *p) {
					state = CLI_PARSE_STATE_SPACE;
				} else {
					argv[argv_idx++] = p;
					state = CLI_PARSE_STATE_CMD;
				}
				break;
			case CLI_PARSE_STATE_SPACE:
				if (' ' == *p) {
					/* keep */
				} else {
					argv[argv_idx++] = p;
					state = CLI_PARSE_STATE_CMD;
				}
				break;
			case CLI_PARSE_STATE_CMD:
				if (' ' == *p) {
					*p = '\0';
					argc++;
					state = CLI_PARSE_STATE_SPACE;
				} else {
					/* keep */
				}
				break;
			default:
				break;
		}
		p++;
	}
	/* handle the last argument */
	if (CLI_PARSE_STATE_CMD == state) {
		argc++;
	}

	if (argc > 0) {
		return cli_cmd_process_root(argc, argv);
	}
	return CLI_PROCESS_RET_SUCCESS;
}

/**
 * @brief     CLI command process function.
 * @param[in] argc: length of command string array.
 * @param[in] argv: the command string array.
 * @return    unsigned char:
 *              255   is successful;
 *              0~254 indicates the position of error command;
 */
unsigned char cli_cmd_process_root(unsigned char argc, unsigned char **argv)
{
	if (!s_strcmp(argv[0], "help")) {
		return cli_cmd_process_help(argc, argv);
	} else if (!s_strcmp(argv[0], "port")) {
		return cli_cmd_process_port(argc, argv);
	} else if (!s_strcmp(argv[0], "profile")) {
		return cli_cmd_process_profile(argc, argv);
	}
	/* Root command error. */
	return 0;
}

/**
 * @brief     'help' command handle function.
 * @param[in] argc: length of command string array.
 * @param[in] argv: the command string array.
 * @return    unsigned char:
 *              255   is successful;
 *              0~254 indicates the position of error command;
 */
unsigned char cli_cmd_process_help(unsigned char argc, unsigned char **argv)
{
	print_str("\r\n");
	if (1 == argc) {
		print_str("help [command]   Display this message or display the details of the command\r\n");
		print_str("port             Port control\r\n");
		print_str("profile          Display or modify the profile\r\n");
	} else if (2 == argc) {
		if (!s_strcmp(argv[1], "port")) {
			print_str("port [port num] on                         Manual mode, turn on the power\r\n");
			print_str("port [port num] off                        Manual mode, turn off the power\r\n");
			print_str("port [port num] bind-profile <name>        Automatic mode, bind a specified profile\r\n");
			print_str("port [port num] unbind-profile             Automatic mode, unbind profile\r\n");
			print_str("port [port num] active                     Activate the bound profile\r\n");
			print_str("port [port num] status                     Display port status\r\n");
		} else if (!s_strcmp(argv[1], "profile")) {
			print_str("profile create <name>                      Create a new profile\r\n");
			print_str("profile delete <name>                      Delete a specified profile\r\n");
			print_str("profile show [name]                        Display profile content\r\n");
			print_str("profile clear <name> <all | line num>      Clear the content of the specified line\r\n");
			print_str("profile modify <name> <line num> <string>  Modify the content of the specified line\r\n");
			print_str("profile save [name]                        Save profile\r\n");
		} else {
			return (unsigned char)(argv[2] - argv[0]);
		}
	} else {
		return (unsigned char)(argv[3] - argv[0]);
	}

	return CLI_PROCESS_RET_SUCCESS;
}

/**
 * @brief     'port' command handle function.
 * @param[in] argc: length of command string array.
 * @param[in] argv: the command string array.
 * @return    unsigned char:
 *              255   is successful;
 *              0~254 indicates the position of error command;
 */
unsigned char cli_cmd_process_port(unsigned char argc, unsigned char **argv)
{
	char channel = -1;

	if (1 == argc) {
		return s_strlen("port") + 1;
	}

	channel = s_atochar(argv[1]);
	if (channel != POWER_CHANNEL_1 && channel != POWER_CHANNEL_2) {
		print_str("\r\nThe port is invalid, please input again.\r\n");
		return CLI_PROCESS_RET_SUCCESS;
	}

	if (!s_strcmp(argv[2], "on")) {
		SET_POWER_STATUS(channel, POWER_ON);
		g_port_status[channel - 1] = POWER_ON;
		g_port_mode[channel - 1] = POWER_CONTROL_MODE_MANUAL;
	} else if (!s_strcmp(argv[2], "off")) {
		SET_POWER_STATUS(channel, POWER_OFF);
		g_port_status[channel - 1] = POWER_OFF;
		g_port_mode[channel - 1] = POWER_CONTROL_MODE_MANUAL;
	} else if (!s_strcmp(argv[2], "bind-profile")) {

	} else if (!s_strcmp(argv[2], "unbind-profile")) {

	} else if (!s_strcmp(argv[2], "active")) {
		g_port_mode[channel - 1] = POWER_CONTROL_MODE_AUTO;
	} else if (!s_strcmp(argv[2], "status")) {
		print_str("\r\n");
		print_str("Port   Mode   Status\r\n");
		print_str("------------------\r\n");
		print_str("1      ");
		print_str((g_port_mode[0] == POWER_CONTROL_MODE_AUTO) ? "Auto" : "Manual");
		print_str((g_port_status[0] == POWER_ON) ? "On" : "Off");
		print_str("\r\n");
		print_str("2      ");
		print_str((g_port_mode[1] == POWER_CONTROL_MODE_AUTO) ? "Auto" : "Manual");
		print_str((g_port_status[1] == POWER_ON) ? "On" : "Off");
		print_str("\r\n");
	} else {
		return (unsigned char)(argv[2] - argv[0]);;
	}

	return CLI_PROCESS_RET_SUCCESS;
}

/**
 * @brief     'profile' command handle function.
 * @param[in] argc: length of command string array.
 * @param[in] argv: the command string array.
 * @return    unsigned char:
 *              255   is successful;
 *              0~254 indicates the position of error command;
 */
unsigned char cli_cmd_process_profile(unsigned char argc, unsigned char **argv)
{
	if (1 == argc) {
		return s_strlen("profile") + 1;
	}

	return CLI_PROCESS_RET_SUCCESS;
}