#ifndef PARSE
#define PARSE

#include <stdio.h>
#include "memory.h"
#include "files.h"

enum types
{
    LABEL,
    DATA,
    EXTERN,
    CODE,
    ENTRY,
    REGISTER,
    NUMBER
};

char *opcode[16] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

int is_type(char *token, int type);
int parse_code(char *tok, char *line,int * parse);

#endif