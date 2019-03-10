#ifndef MEMORY
#define MEMORY

#include <stdio.h>
#include <stdbool.h>
#include "error.h"

#define NUM_DATA (0)
#define CHAR_DATA (1)
#define RELOCATION 2
#define WORD_SIZE (12)
#define MAX_INT (2 ^ (WORD_SIZE - 1))
#define MAX_UINT (2 ^ WORD_SIZE)

enum labels
{
    DATAL = 0,
    CODEL,
    ENTRYL,
    EXTERNL
};


typedef struct node
{
    int type;
    char *name;
    int address;
    struct node *next;
} label_t;

unsigned int code[1024];
unsigned int data[1024];
int DC, IC;
err_node_t *error_list;


int add_data_label(unsigned int line_index, char *fname, char *name,label_t **labels);
int update_data(char *tok, char *line, unsigned int *data, unsigned int line_index, char *fname);
int add_extern_label(unsigned int line_index, char *fname, char *name,label_t **labels);
int add_code_label(unsigned int line_index, char *fname, char *name,label_t **labels);
int update_code(int run, char *tok, char *line_s, unsigned int line_index, char *fname, unsigned int *code,label_t **labels);
int update_entry(unsigned int line_index, char *fname, char *name,label_t **labels);
void add_label(int type, char *name, int address,label_t **labels);

#endif
