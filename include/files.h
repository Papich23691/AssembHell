#ifndef __H_FILES
#define __H_FILES

#include "memory.h"
#include <stdio.h>
#include "error.h"

#define SIX_BITS 63
#define B64_DIG_LEN (65)
#define BIT_MOVE 6

void create_error_file(err_node_t *errors);
void create_files(unsigned int code[1024], unsigned int data[1024], label_t *labels, char *name);
char *bin_to_64(int value, char *conversion);

#endif
