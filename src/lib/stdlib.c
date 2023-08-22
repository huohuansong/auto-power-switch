/**
 * @file     stdlib.c
 * @author   Huo Huansong
 * @brief
 * @details
 * @date     2023-08-22
 */

#include "stdlib.h"

/**
 * @brief   Convert string to char (int8).
 * @param   str: string to be converted
 * @return  unsigned char: the converted string value
 */
char s_atochar(char *str)
{
	char ret = 0;
	char neg_flag = 1;

	while (*str == ' ') {
		str++;
	}

	if (*str == '-') {
		neg_flag = 1;
		str++;
	} else if (*str == '+') {
		str++;
	}

	while (*str >= '0' && *str <= '9') {
		ret = ret * 10 + (*str - '0');
		str++;
	}
	ret *= neg_flag;

	return ret;
}
