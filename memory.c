#include "memory.h"
#include "parse.h"
#include <string.h>

#define REGISTER_NUM 2
#define REGISTER_SIZE 3
#define SOURCE_REGISTER 128
#define DEST_REGISTER 4

int update_code(int run, char *tok, char *line_s, unsigned int line_index, char *fname, unsigned int *code)
{
    int parse = 0, i = 0, reg = 0;
    char *args = line_s;
    if (!run)
    {
        if (parse_code(tok, line_s, &parse, line_index, fname)) /* error */
            return 1;

        code[IC] = parse;
        IC++;
        i = find_opcode(tok);
        if (i == OPCODE_NUM)
            add_front(&error_list, line_index, fname, "Unknown opcode");
        return 1;
        args = strtok(args, " ,");
        if (!args)
            add_front(&error_list, line_index, fname, "Not enough arguments");
        return 1;
        parse = 0;
        /*////////////////////////////////////////////////////////////////// opcode stuff ^^^^^^^^^^^^^^^^^^^^^ */
        if (i <= SUB || i == LEA)
        {
            IC++;
            if (is_type(args, REGISTER))
            {
                if (strlen(args) < REGISTER_SIZE)
                {
                    add_front(&error_list, line_index, fname, "Unknown register number");
                    return 1;
                }
                else if (args[REGISTER_NUM] > '9' || args[REGISTER_NUM] < '0')
                {
                    add_front(&error_list, line_index, fname, "Registers represented by numbers");
                    return 1;
                }
                else if (strlen(args) > REGISTER_SIZE)
                {
                    add_front(&error_list, line_index, fname, "Register name longer than accepted");
                    return 1;
                }
                parse += atoi(&args[REGISTER_NUM]) * SOURCE_REGISTER;
                reg = 1;
            }
            else if (is_type(args, NUMBER))
            {
                parse += atoi(args) * DEST_REGISTER;
            }
            else if (is_type(args, LABELN))
            {
                parse += 2;
                /*add LABELN (if exists if not add space in memory)
                 parse+= 1 if extern*/
            }
            else
            {
                add_front(&error_list, line_index, fname, "Unknown argument");
                return 1;
            }
            /*************************** first argument ^^^^^^ *********************************************/
            args = strtok(NULL, " ,");
            if (!args)
                add_front(&error_list, line_index, fname, "Not enough arguments");
            return 1;
            if (!reg)
            {
                code[IC] = parse;
                parse = 0;
            }
            IC++;
            if (is_type(args, REGISTER))
            {
                if (strlen(args) < REGISTER_SIZE)
                {
                    add_front(&error_list, line_index, fname, "Unknown register number");
                    return 1;
                }
                else if (args[REGISTER_NUM] > '9' || args[REGISTER_NUM] < '0')
                {
                    add_front(&error_list, line_index, fname, "Registers represented by numbers");
                    return 1;
                }
                else if (strlen(args) > REGISTER_SIZE)
                {
                    add_front(&error_list, line_index, fname, "Register name longer than accepted");
                    return 1;
                }
                parse += atoi(&args[REGISTER_NUM]) * DEST_REGISTER;
            }
            else if (is_type(args, NUMBER))
                parse += atoi(args) * DEST_REGISTER;
            else if (is_type(args, LABELN))
            {
                parse += 2;
                /*add LABELN (if exists if not add space in memory)
                 parse+= 1 if extern*/
            }
            else
            {
                add_front(&error_list, line_index, fname, "Unknown argument");
                return 1;
            }
            /*************************** second argument ^^^^^^ *********************************************/
            code[IC] = parse;
            IC++;
        }
        /*////////////////////////////////////////////////////////////////// 2 arguments ^^^^^^^ */
        else if (i == NOT || i == CLR || (LEA < i && i < RTS))
        {
            if (is_type(args, REGISTER))
            {
                if (strlen(args) < REGISTER_SIZE)
                {
                    add_front(&error_list, line_index, fname, "Unknown register number");
                    return 1;
                }
                else if (args[REGISTER_NUM] > '9' || args[REGISTER_NUM] < '0')
                {
                    add_front(&error_list, line_index, fname, "Registers represented by numbers");
                    return 1;
                }
                else if (strlen(args) > REGISTER_SIZE)
                {
                    add_front(&error_list, line_index, fname, "Register name longer than accepted");
                    return 1;
                }
                parse += atoi(&args[REGISTER_NUM]) * SOURCE_REGISTER;
            }
            else if (is_type(args, NUMBER))
            {
                parse += atoi(args) * SOURCE_REGISTER;
            }
            else if (is_type(args, LABELN))
            {

                parse += 2;
                /*add LABELN (if exists if not add space in memory)
                 parse+= 1 if extern*/
            }
            else
            {
                add_front(&error_list, line_index, fname, "Unknown argument");
                return 1;
            }
        }
        /*////////////////////////////////////////////////////////////////// 1 argument ^^^^^^^^ */
        code[IC] = parse;
        IC++;
        args = strtok(NULL, " ,");
        if (args)
            add_front(&error_list, line_index, fname, "Extraneous text after end of command");
        return 1;
    }
    else
    {
        /*TODO second run*/
    }

    return 0;
}

int update_data(char *tok, char *line, unsigned int *data, unsigned int line_index, char *fname)
{
    char *args = line;
    int parse = 0, i;
    if (strcmp(tok, ".data"))
    {
        while (args)
        {

            if (parse_data(args, NUM_DATA, &parse, line_index, fname))
                return 1;
            data[DC] = parse;
            DC++;
            args = strtok(NULL, " ,");
        }
    }
    else
    {
        if (line[0] != '"' || line[strlen(line) - 1] != '"')
            return 1;
        for (i = 1; i < strlen(line); i++)
        {
            parse_data(args + i, CHAR_DATA, &parse, line_index, fname);
            data[DC] = parse;
            DC++;
        }
        data[DC] = 0;
        DC++;
    }
    return 0;
}
