/**
 * @file     string.c
 * @author   Huo Huansong
 * @brief
 * @details
 * @date     2023-08-21
 */

#include "string.h"

/**
 * @brief   Get length of string.
 * @param   str:
 * @return  unsigned char:
 */
unsigned char s_strlen(char *str)
{
	unsigned char len = 0;

	while (*str++) {
		len++;
	}

	return len;
}

/**
 * @brief     Compare str1 with str2.
 * @param[in]   str1:
 * @param[in]   str2:
 * @return  unsigned char:
 */
char s_strcmp(char *str1, char *str2)
{
	char ret = 0;

	while (*str1 && *str1 == *str2) {
		str1++;
		str2++;
	}
	ret = *((unsigned char *)str1) - *((unsigned char *)str2);

	return (ret == 0) ? 0 : ((ret > 0) ? 1 : -1);
}