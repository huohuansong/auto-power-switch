/**
 * @file     cli.h
 * @author   Huo Huansong
 * @brief    Cli function header file.
 * @details
 * @date     2023-08-20
 */

#ifndef _CLI_H_
#define _CLI_H_

/* Macro & Enum */
/* Valid characters */
#define CHR_BACKSPACE                      '\b'  /* 0x08, BS backspace */
#define CHR_CARRIAGE_RETURN                '\r'  /* 0x0D, CR carriage ret */
#define CHR_ESCAPE                         0x1B  /* ESC escape */
#define CHR_QUESTION                       '?'

#define CHR_SYMBOL_1_RANGE_MIN             ' '   /* 0x20 ~ 0x2F */
#define CHR_SYMBOL_1_RANGE_MAX             '/'
#define CHR_NUMBER_RANGE_MIN               '0'   /* 0x30 ~ 0x39 */
#define CHR_NUMBER_RANGE_MAX               '9'
#define CHR_SYMBOL_2_RANGE_MIN             ':'   /* 0x3A ~ 0x40 */
#define CHR_SYMBOL_2_RANGE_MAX             '@'
#define CHR_UPPERCASE_LETTER_RANGE_MIN     'A'   /* 0x41 ~ 0x5A */
#define CHR_UPPERCASE_LETTER_RANGE_MAX     'Z'
#define CHR_SYMBOL_3_RANGE_MIN             ':'   /* 0x5B ~ 0x60 */
#define CHR_SYMBOL_3_RANGE_MAX             '`'
#define CHR_LOWERCASE_LETTER_RANGE_MIN     'a'   /* 0x61 ~ 0x7A */
#define CHR_LOWERCASE_LETTER_RANGE_MAX     'z'
#define CHR_SYMBOL_4_RANGE_MIN             '{'   /* 0x7B ~ 0x7E */
#define CHR_SYMBOL_4_RANGE_MAX             '~'

/* CLI releated */
#define CLI_WELCOME       "\r\nWelcome to the command line.\r\n"
#define CLI_HELP          "You can use 'help' or '?' command to view the help context.\r\n"
#define CLI_PROMPT        "> "

#define CLI_CMDLINE_BUFFER_SIZE   128

/* CLI state machine */
#define CLI_CMDLINE_STATE_INIT                 0
#define CLI_CMDLINE_STATE_NORMAL               1
#define CLI_CMDLINE_STATE_BUSY                 2
#define CLI_CMDLINE_STATE_ESC                  3
#define CLI_CMDLINE_STATE_ESC_LEFT_BRACKET     4
#define CLI_CMDLINE_STATE_STOP                 255

/* Function declaration */
void cli_init(void);
void cli_print_prompt(void);
void cli_print_hex(const unsigned char data);
void cli_print_char(const unsigned char data);
void cli_print_string(unsigned char *str);
void cli_process_char(const unsigned char data);

#endif