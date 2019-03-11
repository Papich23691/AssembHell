#ifndef __H_FILES
#define __H_FILES

#include "error.h"
#include "memory.h"
#include <stdio.h>

#define SIX_BITS 63
#define B64_DIG_LEN (65)
#define BIT_MOVE 6

/**
 * @brief Create a error file object
 *
 * @param head Head of errors stack
 */
void create_error_file(err_node_t *errors);

/**
 * @brief Create the output files
 *
 * @param code Memory of the code
 * @param data Memory of the data
 * @param labels Linked List of labels
 * @param name Name of the files
 */
void create_files(unsigned int code[1024], unsigned int data[1024],
                  label_t *labels, char *name);

/**
 * @brief Translates 12 bits to base64
 *
 * @param value The value which being parsed
 * @param conversion The translated value passes to conversion
 * @return char* a pointer to the translated value
 */
char *bin_to_64(int value, char *conversion);

#endif
