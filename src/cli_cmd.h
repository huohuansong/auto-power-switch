/**
 * @file     cli_cmd.h
 * @author   Huo Huansong
 * @brief    Cli command function header file.
 * @details
 * @date     2023-08-20
 */

#ifndef _CLI_CMD_H_
#define _CLI_CMD_H_

/* Macro & Enum */
#define ARGC_MAX 10

#define CLI_PARSE_STATE_INIT  0
#define CLI_PARSE_STATE_SPACE 1
#define CLI_PARSE_STATE_CMD   2

#define CLI_PROCESS_RET_SUCCESS 255

/* Function declaration */
void cli_cmd_init(void *cb_pr_str);
unsigned char cli_cmd_parse(unsigned char *cmd);
unsigned char cli_cmd_process_root(unsigned char argc, unsigned char **argv);
unsigned char cli_cmd_process_help(unsigned char argc, unsigned char **argv);
unsigned char cli_cmd_process_port(unsigned char argc, unsigned char **argv);
unsigned char cli_cmd_process_profile(unsigned char argc, unsigned char **argv);

#endif