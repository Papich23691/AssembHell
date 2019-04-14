#include "parse.h"
#include "util.h"
#include "memory.h"
#include <ctype.h>
#include <string.h>

#define OPCODE_BITS 32
#define FIRST_BITS 4
#define SECOND_BITS 512
#define DIRECT_REG 5
#define DIRECT 3
#define IMMEDIATE 1
#define MAX_VALUE 4096

/*String array of key words*/
const char *key_words[] = {
    "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec",
    "jmp", "bne", "red", "prn", "jsr", "rts", "stop", "r1", "r2",
    "r3", "r4", "r5", "r6", "r7", "data", "string", "entry", "extern"};

/*
 * @breif Checks if there are too many commas or commas one after anoher
 *
 * @param opcode a number which represents the opcode
 * @param arguments the arguments for the opcode
 * @param data
 *
 * @retrurn if the commas in the opcode are valid
 */
int comma_check(int opcode, char *arguments, int data)
{
  if (arguments)
  {
    int i, comma = 0;
    for (i = 0; i < strlen(arguments); i++)
    {
      if (arguments[i] == ',')
      {
        if (i != strlen(arguments) - 1 && arguments[i] == arguments[i + 1])
        {
          return 1;
        }
        ++comma;
      }
    }
    if (opcode <= SUB || opcode == LEA)
    {
      if (comma < 1 || comma > 1)
        return 1;
    }
    else if (opcode < OPCODE_NUM)
    {
      if (comma > 0)
        return 1;
    }
    else if (opcode == OPCODE_NUM)
    {
      if (comma < data - 1 || comma > data - 1)
        return 1;
    }
  }
  return 0;
}

/*
 * @breif Finds the opcode representetive number
 *
 * @param tok the token (as string)
 * @return the number which represent the opcode
 */
int find_opcode(char *tok)
{
  int i;
  for (i = 0; i < OPCODE_NUM && strcmp(tok, key_words[i]); i++)
    ;
  return i;
}

/*
 * @breif Checks a token by type
 *
 * @param token A given token
 * @param type Type from enum
 *
 * @return true:1 , false:0
 */
int is_type(char *token, int type)
{
  int ret = 0, i;

  if (!token)
    return 0;

  switch (type)
  {
    /* Checks if a chunk is a label */
  case LABEL:
    ret = (token[strlen(token) - 1] == ':');
    break;
    /* Checks if a chunk is a data */
  case DATA:
    ret = (!strcmp(token, ".data") || !strcmp(token, ".string"));
    break;
    /* Checks if a chunk is an extern */
  case EXTERN:
    ret = (!strcmp(token, ".extern"));
    break;
    /* Checks if a chunk is an entry */
  case ENTRY:
    ret = (!strcmp(token, ".entry"));
    break;
  /* Checks if a chunk is a code */
  case CODE:
    for (i = 0; i < OPCODE_NUM; i++)
      ret = ret || (!strcmp(token, key_words[i]));
    break;
  /**/
  case REGISTER:
    ret = (token[0] == '@' && token[1] == 'r');
    break;
  /*Checks if a number is positve or negative and also checks if the given token
   * has numbers in it*/
  case NUMBER:
    ret = (isdigit(token[0]) || token[0] == '+' || token[0] == '-');
    for (i = 1; i < strlen(token); i++)
      ret = ret && isdigit(token[i]);
    break;
  case LABELN:
    ret = is_valid_label(token);
    break;
  default:
    break;
  }
  return ret;
}

/**
 * @brief Parse a line to a create a word representing the opcode
 *
 * @param tok The opcode
 * @param line The arguments
 * @param parse Pointer to the parse
 * @param line_index Index of the current line
 * @param fname Name of the current file
 *
 * @return int The word representing the opcode
 */
int parse_code(char *tok, char *line, unsigned int *parse,
               unsigned int line_index, char *fname)
{

  char *args = duplicate_string(line);
  char *start_of_args = args;
  int i = find_opcode(tok);

  /*Checks if known opcode*/
  if (i >= OPCODE_NUM)
  {
    add_front(&error_list, line_index, fname, "Unknown opcode");
    return 1;
  }
  /*Adds opcode code*/
  *parse = OPCODE_BITS * i;
  args = strtok(args, " , ");
  /**
   * Code for each memory
   * Register:5
   * Label:3
   * Number:1
   */
  if (i <= SUB || i == LEA)
  {
    /*2 arguments*/
    if (is_type(args, REGISTER) && i != LEA)
      *parse += DIRECT_REG * SECOND_BITS;
    else if (is_type(args, NUMBER) && i != LEA)
      *parse += IMMEDIATE * SECOND_BITS;
    else if (is_type(args, LABELN))
      *parse += DIRECT * SECOND_BITS;
    else
    {
      add_front(&error_list, line_index, fname, "Unknown label");
      return 1;
    }
    /* Moves to next arguments */
    args = strtok(NULL, " , ");
    if (is_type(args, NUMBER) && i != CMP)
    {
      add_front(&error_list, line_index, fname,
                "Can't pass number as an argument");
      return 1;
    }
    else if (is_type(args, NUMBER) && i == CMP)
      *parse += IMMEDIATE * FIRST_BITS;
    else if (is_type(args, REGISTER))
      *parse += DIRECT_REG * FIRST_BITS;
    else if (is_type(args, LABELN))
      *parse += DIRECT * FIRST_BITS;
    else
    {
      add_front(&error_list, line_index, fname, "Unknown label");
      return 1;
    }
  }
  else if (i == NOT || i == CLR || (LEA < i && i < RTS))
  {
    /*1 arguments*/
    if (is_type(args, NUMBER) && i != PRN)
    {
      add_front(&error_list, line_index, fname,
                "Can't pass number as an argument");
      return 1;
    }
    else if (is_type(args, NUMBER) && i == PRN)
      *parse += IMMEDIATE * FIRST_BITS;
    else if (is_type(args, REGISTER))
      *parse += DIRECT_REG * FIRST_BITS;
    else if (is_type(args, LABELN))
      *parse += DIRECT * FIRST_BITS;
    else
    {
      add_front(&error_list, line_index, fname, "Unknown label");
      return 1;
    }
  }

  free(start_of_args);
  return 0;
}

/**
 * @breif return the binary code of a given data type
 *
 *
 * @param tok A char or a number
 * @param data_type 1 if string 0 if number data
 * @param parse A pointer to the parse
 * @param line_index Index of the current line
 * @param fname Name of the current file
 *
 * @retrun 1 if error
 */
int parse_data(char *tok, int data_type, unsigned int *parse,
               unsigned int line_index, char *fname)
{
  if (data_type)
  {
    *parse = (int)*tok;
  }
  else
  {
    if (is_type(tok, NUMBER) && atoi(tok) < MAX_VALUE)
    {
      *parse = atoi(tok);
    }
    else
    {
      add_front(&error_list, line_index, fname, "Wrong data type");
      return 1;
    }
  }
  return 0;
}

/**
 * @breif checks if a given label is valid
 *
 * @param label string of a label
 * @return if the label is valid
 */
int is_valid_label(char *label)
{
  int i;

  if (!label || !IS_ALPHABET(label[0]))
    return 0;
  /* Make sure the label is valid. */
  for (i = 1; i < strlen(label); i++)
  {
    if (!IS_ALPHABET(label[i]) && !IS_NUMERIC(label[i]))
    {
      return 0;
    }
  }
  /* Check if the user used a keyword */
  for (i = 0; i < CNT_KWORDS; i++)
  {
    if (!strcmp(label, key_words[i]))
      return 0;
  }
  return 1;
}

/**
 * @brief  Parse an operand to a create a word representing the operand
 * 
 * @param args The operand
 * @param operand_num Number of operands
 * @param parse Pointer to the parsed word
 * @param first_register Determines if the first operand is a register
 * @param line_index Index of the current line
 * @param fname Name of the current file
 * @return 1 if error
 */
int first_cycle_parse_operand(char * args,int operand_num, unsigned int * parse, unsigned int * first_register, int line_index,char * fname)
{
   if (!args)
            {
                add_front(&error_list, line_index, fname, "Not enough arguments");
                return 1;
            }
            /* Register operand */
            if (is_type(args, REGISTER))
            {
                if (strlen(args) < REGISTER_SIZE)
                {
                    add_front(&error_list, line_index, fname, "Unknown register number");
                    return 1;
                }
                else if (args[REGISTER_NUM] > '9' || args[REGISTER_NUM] < '0')
                {
                    add_front(&error_list, line_index, fname,
                              "Registers represented by numbers");
                    return 1;
                }
                else if (strlen(args) > REGISTER_SIZE)
                {
                    add_front(&error_list, line_index, fname,
                              "Register name longer than accepted");
                    return 1;
                }
                if (--operand_num)
                  *parse += atoi(&args[REGISTER_NUM]) * SOURCE_REGISTER;
                else
                  *parse += atoi(&args[REGISTER_NUM]) * DEST_REGISTER;
                *first_register = 1;
            }
            /* Number operand */
            else if (is_type(args, NUMBER))
            {
                if (*first_register)
                {
                    code[IC] = *parse;
                    ++IC;
                }
                *parse=0;
                *parse += atoi(args) * DEST_REGISTER;
            }
            /* Label operand */
            else if (is_type(args, LABELN))
            {
              if (*first_register)
                {
                    code[IC] = *parse;
                    ++IC;
                }
                parse = 0;
            }
            else
            {
                add_front(&error_list, line_index, fname, "Unknown argument");
                return 1;
            }
            return 0;
}

/**
 * @brief Parses labels using the label table created in first cycle
 * 
 * @param args The operand
 * @param operand_num Number of operands
 * @param parse Pointer to the parsed word
 * @param first_register Determines if the first operand is a register
 * @param curr Pointer to the head of the labels table
 * @param line_index Index of the current line
 * @param fname Name of the current file
 * @return 1 if error
 */
int second_cycle_parse_operand(char * args,int operand_num, unsigned int * parse, unsigned int * first_register, label_t **curr,int line_index,char * fname){
            /* Label operand */
            if (is_type(args, LABELN))
            {
                while (*curr)
                {
                     /* If extern - passes only 1 */
                    if (!strcmp(args, (*curr)->name) && (*curr)->type == EXTERNL)
                    {
                        add_label(EXTERNL, args, IC + START, &ext);
                        *parse += 1;
                        break;
                    }
                    /* Passes space in memory */
                    else if (!strcmp(args, (*curr)->name))
                    {
                        *parse += RELOCATION;
                        *parse += (*curr)->address * DEST_REGISTER;
                        break;
                    }
                    curr = &(*curr)->next;
                }
                if (!*curr)
                {
                    add_front(&error_list, line_index, fname, "Unkown label");
                    return 1;
                }
                if (*first_register)
                    ++IC;
                code[IC] = *parse;
            }
            /* If the operand it's not a register and if it's the first operand or whether the first operand was a register */
            if (!is_type(args, REGISTER) && ((operand_num-1) || *first_register))
                ++IC;
            /* First - register operand */
            else if (operand_num-1)
                *first_register=1;
            else
              ++IC;
            return 0;
}
