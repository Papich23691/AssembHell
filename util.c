#include "util.h"

char *duplicate_string(const char *str) {
    char *new_str;
    if (!str) return NULL;
    
    new_str = (char *) malloc(strlen(str) + 1);
    strcpy(new_str, str);

    return new_str;
}
