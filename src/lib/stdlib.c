/**
 * @file     stdlib.c
 * @author   Huo Huansong
 * @brief
 * @details
 * @date     2023-08-22
 */

#include "stdlib.h"

/**
 * @brief     Convert string to char (int8).
 * @param[in] str: string to be converted
 * @return    unsigned char: the converted string value
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

/**
 * @brief      Convert char (int8) to string.
 * @param[in]  val: char (int8) to be converted
 * @param[out] str: the output string
 * @return     none
 */
void s_chartoa(char val, char *str)
{
	char *p = str;

	if (val < 0) {
		*p++ = '-';
		val = 0 - val;
	}

	if (val < 10) {
		*p++ = '0' + val;
	} else if (val >= 10 && val < 100) {
		*p++ = '0' + (val / 10);
		*p++ = '0' + (val % 10);
	} else {
		*p++ = '0' + (val / 100);
		*p++ = '0' + ((val % 100) / 10);
		*p++ = '0' + (val % 10);
	}
	*p = 0;
}
