#include "parse.h"
#include <string.h>

#define OPCODE_BITS 32
#define FIRST_BITS 4
#define SECOND_BITS 1024

int is_type(char *token, int type)
{
    int ret = 0, i;
    switch (type)
    {
    case LABEL:
        ret = (token[strlen(token) - 1] == ':') ? 1 : 0;
        break;
    case DATA:
        ret = (!strcmp(token, ".data") || !strcmp(token, ".string")) ? 1 : 0;
        break;
    case EXTERN:
        ret = (!strcmp(token, ".extern")) ? 1 : 0;
        break;
    case ENTRY:
        ret = (!strcmp(token, ".entry")) ? 1 : 0;
        break;
    case CODE:
        for (int i = 0; i < 16; i++)
            ret |= (!strcmp(token, opcode[i])) ? 1 : 0;
        break;
    case REGISTER:
        ret = (token[0] == '@' && token[1] == 'r') ? 1 : 0;
        break;
    case NUMBER:
        ret = (isdigit(token[0]) || token[0] == '+' || token[0] == '-') ? 1 : 0;
        for (int i = 1; i < strlen(token); i++)
            ret &= isdigit(token[i]);
        break;
    default:
        break;
    }
    return ret;
}

int parse_code(char *tok, char *line, int *parse)
{
    int i;
    char *args = line;
    for (i = 0; i < 16; i++)
    {
        if (!strcmp(tok, opcode[i]))
            break;
    }
    if (i == 16)
        return 1; //error
    parse += OPCODE_BITS * i;
    args = strtok(line, " ,");
    if (i <= 3 || i == 6)
    {
        if (is_type(args, REGISTER))
            parse += 5 * FIRST_BITS;
        //else if () num and labels for first 
               
        //2 arguments
    }
    else if (4 == i || i == 5 || (6 < i && i < 14))
    {
        if (is_type(args, REGISTER))
            parse += 5 * FIRST_BITS;
        //else if () num and labels for first 
        //1 argument
    }
    return 0;
}