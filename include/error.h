#ifndef __H_ERROR
#define __H_ERROR

#include "util.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Node of error stack
 *
 */
typedef struct ERROR_NODE {
  unsigned int line;
  char *fname;
  char *desc;

  struct ERROR_NODE *next;
} err_node_t;

/**
 * @brief Adds a node to error stack
 *
 * @param head Head of the stack
 * @param line Index of line in which there was an error
 * @param fname Name of the file in which there was an error
 * @param desc Description of the error
 */
void add_front(err_node_t **head, unsigned int line, char *fname, char *desc);

/**
 * @brief Deletes error list from memory
 *
 * @param root Root of the stack
 */
void delete_errors_list(err_node_t **head);

#endif /* __H_ERROR */
