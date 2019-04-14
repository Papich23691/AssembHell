#ifndef __H_UTIL
#define __H_UTIL

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * @breif duplicates a given string to a new one
 *
 * @param str the string to duplicate
 * @return the duplicated string
 */
char *duplicate_string(const char *str);

/**
 * Prints stylish messages 
 */
void print_message(char * message_info , int status);

#endif
