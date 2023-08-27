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
#include "port.h"
#include "cli.h"
#include "cli_cmd.h"

#include "lib/string.h"
#include "lib/stdlib.h"

/* Global */
static void (*print_str)(unsigned char *str) = NULL;

/**
 * @brief     CLI command initialization function.
 * @param[in] none
 * @return    none
 */
void cli_cmd_init(void *cb_pr_str)
{
	print_str = cb_pr_str;
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
			// print_str("profile create <name>                      Create a new profile\r\n");
			// print_str("profile delete <name>                      Delete a specified profile\r\n");
			print_str("profile show [name]                        Display profile content\r\n");
			print_str("profile clear <name> <all | line num>      Clear the content of the specified line\r\n");
			print_str("profile modify <name> <line num> <string>  Modify the content of the specified line\r\n");
			print_str("profile save [name]                        Save profile\r\n");
		} else {
			return (unsigned char)(argv[1] - argv[0]);
		}
	} else {
		return (unsigned char)(argv[2] - argv[0]);
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
	unsigned char i = 0;
	char user_port = -1;
	unsigned char flag_port_all = FALSE;
	unsigned char action_argv_index = 2;  /* for all port */
	struct port_attr attr = {0};
	char str[3] = {0};

	if (1 == argc) {
		return s_strlen("port") + 1;
	}

	user_port = s_atochar(argv[1]);
	for (i = 0; i < PORT_NUM; i++) {
		if (GET_PHYSICAL_PORT_FROM_USER_PORT(user_port) == i) {
			break;
		}
	}
	if (i == PORT_NUM) {
		flag_port_all = TRUE;
		action_argv_index = 1;
	}

	if (!s_strcmp(argv[action_argv_index], "on")) {
		if (TRUE == flag_port_all) {
			for (i = 0; i < PORT_NUM; ++i) {
				port_set_power_status(i, PORT_POWER_ON);
			}
		} else {
			port_set_power_status(GET_PHYSICAL_PORT_FROM_USER_PORT(user_port), PORT_POWER_ON);
		}
	} else if (!s_strcmp(argv[action_argv_index], "off")) {
		if (TRUE == flag_port_all) {
			for (i = 0; i < PORT_NUM; ++i) {
				port_set_power_status(i, PORT_POWER_OFF);
			}
		} else {
			port_set_power_status(GET_PHYSICAL_PORT_FROM_USER_PORT(user_port), PORT_POWER_OFF);
		}
	} else if (!s_strcmp(argv[action_argv_index], "bind-profile")) {

	} else if (!s_strcmp(argv[action_argv_index], "unbind-profile")) {

	} else if (!s_strcmp(argv[action_argv_index], "active")) {

	} else if (!s_strcmp(argv[action_argv_index], "status")) {
		print_str("\r\n");
		print_str("Port   Mode   Status\r\n");
		print_str("--------------------\r\n");
		if (TRUE == flag_port_all) {
			for (i = 0; i < PORT_NUM; ++i) {
				port_get_port_status(i, &attr);
				s_chartoa(GET_UESR_PORT_FROM_PHYSICAL_PORT(i), str);
				print_str(str);
				print_str("      ");
				print_str((attr.mode == PORT_POWER_CONTROL_MODE_AUTO) ? "Auto" : "Manual");
				print_str(" ");
				print_str((attr.running_status == PORT_POWER_ON) ? "On" : "Off");
				print_str("\r\n");
			}
		} else {
			s_chartoa(user_port, str);
			print_str(str);
			print_str("      ");
			print_str((attr.mode == PORT_POWER_CONTROL_MODE_AUTO) ? "Auto" : "Manual");
			print_str(" ");
			print_str((attr.running_status == PORT_POWER_ON) ? "On" : "Off");
			print_str("\r\n");
		}
	} else {
		return (unsigned char)(argv[action_argv_index] - argv[0]);
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