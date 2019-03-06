#ifndef PARSE
#define PARSE

#include <stdio.h>
#include "memory.h"
#include "files.h"

#define OPCODE_NUM 16

enum types
{
    LABEL = 0,
    DATA,
    EXTERN,
    CODE,
    ENTRY,
    REGISTER,
    NUMBER,
    LABELN
};

enum opcodes
{
    MOV = 0,
    CMP,
    ADD,
    SUB,
    NOT,
    CLR,
    LEA,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP
};

char *opcode[OPCODE_NUM] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"};

int is_type(char *token, int type);
int parse_code(char *tok, char *line,int * parse);
int find_opcode(char *tok);
int parse_data(char *tok, int data_type, int *parse);


#endif