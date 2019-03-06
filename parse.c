#include "parse.h"
#include "memory.h"
#include <string.h>

#define OPCODE_BITS 32
#define FIRST_BITS 4
#define SECOND_BITS 1024
#define DIRECT_REG 5
#define DIRECT 3
#define IMMEDIATE 1
#define MAX_VALUE 4096

int find_opcode(char *tok)
{
    int i;
    for (i = 0; i < OPCODE_NUM && strcmp(tok, opcode[i]); i++);

    return i;
}

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
        for (i = 0; i < OPCODE_NUM; i++)
            ret |= (!strcmp(token, opcode[i])) ? 1 : 0;
        break;
    case REGISTER:
        ret = (token[0] == '@' && token[1] == 'r') ? 1 : 0;
        break;
    case NUMBER:
        ret = (isdigit(token[0]) || token[0] == '+' || token[0] == '-') ? 1 : 0;
        for (i = 1; i < strlen(token); i++)
            ret &= isdigit(token[i]);
        break;
    case LABELN:
        ret = ((token[0] >= 'A' && token[0] <= 'Z') || (token[0] >= 'a' && token[0] <= 'z')) ? 1 : 0;
        for (i = 1; i < strlen(token); i++)
            ret &= ((token[1] >= 'A' && token[i] <= 'Z') || (token[i] >= 'a' && token[i] <= 'z') || (token[i] >= '0' && token[i] <= '9')) ? 1 : 0;
        break;
    default:
        break;
    }
    return ret;
}

int parse_code(char *tok, char *line, int *parse)
{
    char *args = line;
    int i = find_opcode(tok);
    
    if (i == OPCODE_NUM) /* error */
        return 1;
        
    parse += OPCODE_BITS * i;
    args = strtok(args, " ,");
    if (i <= SUB || i == LEA)
    {
        if (is_type(args, REGISTER) && i != LEA)
            parse += DIRECT_REG * SECOND_BITS;
        else if (is_type(args, NUMBER) && i != LEA)
            parse += IMMEDIATE * SECOND_BITS;
        else if (is_type(args, LABELN))
            parse += DIRECT * SECOND_BITS;
        else
        {
            /* error, unkown label  */
            return 1;
        }
        args = strtok(NULL, " ,");
        if (is_type(args, NUMBER) && i != CMP)
        {
            /* cant accept number */
            return 1;
        }
        else if (is_type(args, NUMBER) && i == CMP)
            parse += IMMEDIATE * FIRST_BITS;
        else if (is_type(args, REGISTER))
            parse += DIRECT_REG * FIRST_BITS;
        else if (is_type(args, LABELN))
            parse += DIRECT * FIRST_BITS;
        else
        {
            /* error, unkown label  */
            return 1;
        }

        //2 arguments
    }
    else if (i == NOT || i == CLR || (LEA < i && i < RTS))
    {
        if (is_type(args, NUMBER) && i != PRN)
        {
            /* cant accept number */
            return 1;
        }
        else if (is_type(args, NUMBER) && i == PRN)
            parse += IMMEDIATE * FIRST_BITS;
        else if (is_type(args, REGISTER))
            parse += DIRECT_REG * FIRST_BITS;
        else if (is_type(args, LABELN))
            parse += DIRECT * FIRST_BITS;
        else
        {
            /* error, unkown label  */
            return 1;
        }

        //1 argument
    }
    return 0;
}

int parse_data(char *tok, int data_type, int *parse)
{
    if (data_type)
    {
        parse += (int)*tok;
    }
    else
    {
        if (is_type(tok, NUMBER) && atoi(tok) < MAX_VALUE) //TODO check number of bits
            *parse += atoi(tok);
        else
            //error
            return 1;
    }
    return 0;
}
