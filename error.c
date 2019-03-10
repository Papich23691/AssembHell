#include "error.h"
#include "util.h"

void add_front(err_node_t **head, unsigned int line, char *fname, char *desc)
{
    err_node_t *new_node = (err_node_t *)malloc(sizeof(err_node_t));
    new_node->line = line;
    new_node->desc = duplicate_string(desc);
    new_node->fname = duplicate_string(fname);
    new_node->next = NULL;

    if (!(*head))
    {
        new_node->next = NULL;
        (*head) = new_node;
    }
    else
    {
        new_node->next = (*head);
        (*head) = new_node;
    }
}

void display_list(err_node_t *head)
{
    err_node_t *current_node = head;
    while (current_node)
    {
        printf("Error\n%s.\nFile: %s\tLine: %u\n", current_node->desc, current_node->fname, current_node->line);
        current_node = current_node->next;
    }
}

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
