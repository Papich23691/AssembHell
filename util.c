#include "util.h"

char *duplicate_string(const char *str) {
    if (!str) return NULL;
    
    char *new_str = (char *) malloc(strlen(str) + 1);
    strcpy(new_str, str);

    return new_str;
}