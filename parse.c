#include "parse.h"
#include "memory.h"
#include <string.h>
#include <ctype.h>

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
    for (i = 0; i < OPCODE_NUM && strcmp(tok, key_words[i]); i++);

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
            ret |= (!strcmp(token, key_words[i])) ? 1 : 0;
        break;
    case REGISTER:
        ret = (token[0] == '@' && token[1] == 'r') ? 1 : 0;
        break;
    case NUMBER:
        ret = ((isdigit(token[0]) || token[0] == '+' || token[0] == '-')) ? 1 : 0;
        for (i = 1; i < strlen(token); i++)
            ret &= isdigit(token[i]);

        break;
    case LABELN:
        ret = is_valid_label(token) ? 1 : 0;
        break;
    default:
        break;
    }
    return ret;
}

int parse_code(char *tok, char *line, int *parse,unsigned int line_index,char *fname)
{
    char *args = line;
    int i = find_opcode(tok);
    
    if (i >= OPCODE_NUM){
        add_front(&error_list,line_index,fname,"Unknown opcode");
        return 1;
    }
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
            add_front(&error_list,line_index,fname,"Unknown label");
            return 1;
        }
        args = strtok(NULL, " ,");
        if (is_type(args, NUMBER) && i != CMP)
        {
            add_front(&error_list,line_index,fname,"Can't pass number as an argument");
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
            add_front(&error_list,line_index,fname,"Unknown label");
            return 1;
        }

        //2 arguments
    }
    else if (i == NOT || i == CLR || (LEA < i && i < RTS))
    {
        if (is_type(args, NUMBER) && i != PRN)
        {
            add_front(&error_list,line_index,fname,"Can't pass number as an argument");
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
            add_front(&error_list,line_index,fname,"Unknown label");
            return 1;
        }

        //1 argument
    }
    return 0;
}

int parse_data(char *tok, int data_type, int *parse,unsigned int line_index,char *fname)
{
    if (data_type)
    {
        parse += (int)*tok;
    }
    else
    {
        if (is_type(tok, NUMBER) && atoi(tok) < MAX_VALUE) 
            *parse += atoi(tok);
        else
            add_front(&error_list,line_index,fname,"Wrong data type");
            return 1;
    }
    return 0;
}


bool is_valid_label(char *label)
{
    int i;

    if (!label || !IS_ALPHABET(label[0]))
        return false;

    /* Make sure the label is valid. */
    for (i = 1; i < strlen(label); ++i)
    {
        if (!IS_ALPHABET(label[i]) || !IS_NUMERIC(label[i]))
            return false;
    }

    /* Check if the user used a keyword */
    for (i = 0; i < CNT_KWORDS; ++i)
    {
        if (!strcmp(label, key_words[i]))
            return false;
    }

    return true;
}
