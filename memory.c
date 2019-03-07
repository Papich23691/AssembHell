#include "memory.h"
#include "parse.h"

#define REGISTER_NUM 2
#define REGISTER_SIZE 3
#define SOURCE_REGISTER 128
#define DEST_REGISTER 4

int update_code(int run, char *tok, char *line_s, unsigned int line_index, char *fname,int *code)
{
    int parse = 0, i = 0;

    if (!run)
    {
        if (parse_code(tok, line_s, &parse)) /* error */
            return 1;

        code[IC]=parse;
        IC++;
        i = find_opcode(tok);
        if (i == OPCODE_NUM)
            //error UNKOWN OPCODE
            return 1;
        char *args = line_s;
        args = strtok(args, " ,");
        if (!args)
            //not enough arguments
            return 1;
        parse = 0;
        //////////////////////////////////////////////////////////////////// opcode stuff ^^^^^^^^^^^^^^^^^^^^^ //
        if (i <= SUB || i == LEA)
        {
            IC++;
            int reg = 0;
            if (is_type(args, REGISTER))
            {
                if (strlen(args) < REGISTER_SIZE)
                    //unkown register number
                    return 1;
                else if (args[REGISTER_NUM] > '9' || args[REGISTER_NUM] < '0')
                    //register is only number based
                    return 1;
                else if (strlen(args) > REGISTER_SIZE)
                    // to much shit after @r
                    return 1;
                parse += atoi(args[REGISTER_NUM]) * SOURCE_REGISTER;
                reg = 1;
            }
            else if (is_type(args, NUMBER)) //TODO check number of bits
            {
                parse += atoi(args) * DEST_REGISTER;
            }
            else if (is_type(args, LABELN))
            {
                parse += 2;
                //add LABELN (if exists if not add space in memory)
                // parse+= 1 if extern
            }
            else
            {
                //error
                return 1;
            }
            /*************************** first argument ^^^^^^ *********************************************/
            args = strtok(NULL, " ,");
            if (!args)
                //not enough arguments
                return 1;
            if (!reg)
            {
                code[IC]=parse;
                parse = 0;
            }
            IC++;
            if (is_type(args, REGISTER))
            {
                if (strlen(args) < REGISTER_SIZE)
                    //unkown register number
                    return 1;
                else if (args[REGISTER_NUM] > '9' || args[REGISTER_NUM] < '0')
                    //register is only number based
                    return 1;
                else if (strlen(args) > REGISTER_SIZE)
                    // to much shit after @r
                    return 1;
                parse += atoi(args[REGISTER_NUM]) * DEST_REGISTER;
            }
            else if (is_type(args, NUMBER)) //TODO check number of bits
            {

                parse += atoi(args) * DEST_REGISTER;
            }
            else if (is_type(args, LABELN))
            {
                parse += 2;
                //add LABELN (if exists if not add space in memory)
                // parse+= 1 if extern
            }
            else
            {
                //error
                return 1;
            }
            /*************************** second argument ^^^^^^ *********************************************/
            code[IC]=parse;
            IC++;
        }
        //////////////////////////////////////////////////////////////////// 2 arguments ^^^^^^^ //
        else if (i == NOT || i == CLR || (LEA < i && i < RTS))
        {
            if (is_type(args, REGISTER))
            {
                if (strlen(args) < REGISTER_SIZE)
                    //unkown register number
                    return 1;
                else if (args[REGISTER_NUM] > '9' || args[REGISTER_NUM] < '0')
                    //register is only number based
                    return 1;
                else if (strlen(args) > REGISTER_SIZE)
                    // to much shit after @r
                    return 1;
                parse += atoi(args[REGISTER_NUM]) * SOURCE_REGISTER;
            }
            else if (is_type(args, NUMBER))
            {
                parse += atoi(args) * SOURCE_REGISTER;
            }
            else if (is_type(args, LABELN))
            {

                parse += 2;
                //add LABELN (if exists if not add space in memory)
                // parse+= 1 if extern
            }
            else
            {
                //error
                return 1;
            }
        }
        //////////////////////////////////////////////////////////////////// 1 argument ^^^^^^^^ //
        code[IC]=parse;
        IC++;
        args = strtok(NULL, " ,");
        if (args)
            //error אחשלי
            return 1;
    }
    else
    {
        //TODO second run
    }

    return 0;
}

int update_data(char *tok, char *line,int *data)
{
    char *args = line;
    int parse = 0;
    if (strcmp(tok, ".data"))
    {
        while (args)
        {

            if (parse_data(args, NUM_DATA, &parse))
                return 1;
            data[DC]=parse;
            DC++;
            args = strtok(NULL, " ,");
        }
    }
    else
    {
        if (line[0] != '"' || line[strlen(line) - 1] != '"')
            return 1;
        int i;
        for (i = 1; i < strlen(line); i++)
        {
            parse_data(args + i, CHAR_DATA, &parse);
            data[DC]=parse;
            DC++;
        }
        data[DC]=0;
        DC++;
    }
    return 0;
}
