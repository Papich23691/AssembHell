#ifndef PARSE
#define PARSE

#include <stdio.h>
#include "files.h"
#include "error.h"

#define OPCODE_NUM 16
#define IS_ALPHABET(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
#define IS_NUMERIC(x) ((x >= '0' && x <= '9'))
#define CNT_KWORDS (27)

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

int is_type(char *token, int type);
int parse_code(char *tok, char *line,unsigned int * parse,unsigned int line_index,char *fname);
int find_opcode(char *tok);
int parse_data(char *tok, int data_type, unsigned int *parse,unsigned int line_index,char *fname);
bool is_valid_label(char *label);

#endif
