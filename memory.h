#ifndef MEMORY
#define MEMORY

#include <stdio.h>
#include <stdbool.h>
#include "error.h"

#define NUM_DATA    (0)
#define CHAR_DATA   (1)
#define WORD_SIZE   (12)
#define MAX_INT     (2 ^ (WORD_SIZE - 1))
#define MAX_UINT    (2 ^ WORD_SIZE)

enum labels
{
    REG = 0,
    ENT,
    EXT
};

unsigned int code[1024];
unsigned int data[1024];
int DC, IC;
err_node_t *error_list;

typedef struct l {
    int type;
    char * name;
    int address;
    int value:WORD_SIZE;
    struct l * next;
} label_t;

int add_data_label(char * line, label_t* labels);
int update_data(char * tok,char * line, unsigned int *data,unsigned int line_index,char *fname);
int add_extern_label(char * line);
int add_code_label(char * line);
int update_code(int run,char * tok, char * line_s , unsigned int line_index, char *fname,unsigned int *code);
int update_entry(char * line);


#endif
