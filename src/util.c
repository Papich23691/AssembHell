#include "util.h"

/*
 * @breif duplicates a given string to a new one
 *
 * @param str the string to duplicate
 * @return the duplicated string
 */
char *duplicate_string(const char *str) {
  char *new_str;
  if (!str)
    return NULL;

  new_str = (char *)malloc(strlen(str) + 1);
  strcpy(new_str, str);

  return new_str;
}

/**
 * Prints stylish messages 
 */
void print_message(char * message_info , int status){
  if (status)
    printf("\033[0;31mError - %s\033[0m\n",message_info);
  else
    printf("\033[0;32m Successfuly assembled %s\033[0m\n", message_info);
}
