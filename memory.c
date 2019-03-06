#include "memory.h"
#include "parse.h"

#define REGISTER_NUM 2
#define REGISTER_SIZE 3
#define SOURCE_REGISTER 128
#define DEST_REGISTER 4

int DC, IC;
extern int line;
extern char *filename;

int update_code(int run, char *tok, char *line, node *Code)
{
    int parse = 0, i = 0;

    if (!run)
    {
        if (parse_code(tok, line, &parse)) /* error */
            return 1;

        /* add opcode parse to data base */
        i = find_opcode(tok);
        if (i == OPCODE_NUM)
            //error UNKOWN OPCODE
            return 1;
        char *args = line;
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
                //add to database
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
            //add to database
        }
        //////////////////////////////////////////////////////////////////// 2 arguments ^^^^^^^ //
        else if (i == NOT || i == CLR || (LEA < i && i < RTS))
        {
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
        args = strtok(NULL, " ,");
        if (strlen(args))
            //error אחשלי
            return 1;
    }
    else
    {
        //TODO second run
    }

    return 0;
}

int update_data(char *tok, char *line, node *Data)
{
    char *args = line;
    int parse = 0;
    if (strcmp(tok, ".data"))
    {
        while (args)
        {

            if (parse_data(args, NUM_DATA, &parse))
                return 1;
            DC++;
            //TODO add to database
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
            //TODO add to database
        }
        //TODO add '\0' to database
    }
    return 0;
}