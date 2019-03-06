#ifndef __H_FILES
#define __H_FILES

#include "memory.h"
#include <stdio.h>

#define SIX_BITS 63
#define B64_DIG_LEN (65)
#define BIT_MOVE 6

void create_files(node *code, node *data, label *labels, char *name);
char *bin_to_64(int value, char *conversion);

#endif