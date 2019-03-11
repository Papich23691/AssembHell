#include "error.h"
#include "util.h"

/**
 * @brief Adds a node to error stack
 *
 * @param head Head of the stack
 * @param line Index of line in which there was an error
 * @param fname Name of the file in which there was an error
 * @param desc Description of the error
 */
void add_front(err_node_t **head, unsigned int line, char *fname, char *desc) {
  err_node_t *new_node = (err_node_t *)malloc(sizeof(err_node_t));
  new_node->line = line;
  new_node->desc = duplicate_string(desc);
  new_node->fname = duplicate_string(fname);
  new_node->next = NULL;

  if (!(*head)) {
    new_node->next = NULL;
    (*head) = new_node;
  } else {
    new_node->next = (*head);
    (*head) = new_node;
  }
}

/**
 * @brief Deletes error list from memory
 *
 * @param root Root of the stack
 */
void delete_errors_list(err_node_t **root) {
  err_node_t *curr = *root, *tmp = NULL;

  while (curr != NULL) {
    tmp = curr;
    curr = curr->next;
    free(tmp->desc);
    free(tmp->fname);
    free(tmp);
  }

  *root = NULL;
}
