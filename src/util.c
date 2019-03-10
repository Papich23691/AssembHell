#include "util.h"


/*
* @breif duplicates a given string to a new one
* 
* @param str the string to duplicate
* @return the duplicated string
*/
char *duplicate_string(const char *str) {
    char *new_str;
    if (!str) return NULL;
    
    new_str = (char *) malloc(strlen(str) + 1);
    strcpy(new_str, str);

    return new_str;
}
