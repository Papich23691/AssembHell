#ifndef PARSE
#define PARSE

#include <stdio.h>
#include "memory.h"
#include "files.h"

#define OPCODE_NUM 16
#define IS_ALPHABET(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x >= 'Z'))
#define IS_NUMERIC(x) ((x >= '0' && x <= '9'))
#define CNT_KWORDS (28)

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

char *key_words[] = {"mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "data", "string", "entry", "extern"};

int is_type(char *token, int type);
int parse_code(char *tok, char *line,int * parse);
int find_opcode(char *tok);
int parse_data(char *tok, int data_type, int *parse);
bool is_valid_label(char *label);

#endif