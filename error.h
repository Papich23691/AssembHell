#ifndef __H_ERROR
#define __H_ERROR

#include <stdio.h>
#include <stdlib.h>

typedef struct ERROR_NODE {
    unsigned int line;
    char *fname;
    char *desc;

    struct ERROR_NODE *next;
} err_node_t;

void add_front(err_node_t **head, unsigned int line, char *fname, char *desc);
void display_list(err_node_t *head);

#endif /* __H_ERROR */
