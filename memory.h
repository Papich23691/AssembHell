#ifndef MEMORY
#define MEMORY

#include <stdio.h>
#include "files.h"


typedef struct n {
    int value:12;
    struct n * next;
} node;

typedef struct l {
    char * type;
    char * name;
    int value:12;
    struct l * next;
} label;

int add_data_label(char * line,int DC,label * labels);
int update_data(char * tok,char * line,int * DC,node * Data);
int add_extern_label(char * line);
int add_code_label(char * line,int IC);
int update_code(int run,char * tok,char * line,int * IC,node * Code);
int update_entry(char * line);

#endif