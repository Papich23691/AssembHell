#ifndef MEMORY
#define MEMORY

#include <stdio.h>
#include "files.h"

#define NUM_DATA    (0)
#define CHAR_DATA   (1)
#define WORD_SIZE   (12)
#define MAX_INT     (2 ^ (WORD_SIZE - 1))
#define MAX_UINT    (2 ^ WORD_SIZE)

typedef struct n {
    int value:WORD_SIZE;
    struct n * next;
} node_t;

typedef struct l {
    char * type;
    char * name;
    int address;
    int value:WORD_SIZE;
    struct l * next;
} label_t;

int add_data_label(char * line, label_t* labels);
int update_data(char * tok,char * line, node * Data);
int add_extern_label(char * line);
int add_code_label(char * line);
int update_code(int run,char * tok, char * line_s , unsigned int line_index, char *fname);
int update_entry(char * line);

#endif