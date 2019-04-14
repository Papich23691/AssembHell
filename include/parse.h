#ifndef __H_PARSE
#define __H_PARSE

#include "error.h"
#include "files.h"
#include <stdio.h>

#define OPCODE_NUM 16
#define IS_ALPHABET(x) ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
#define IS_NUMERIC(x) ((x >= '0' && x <= '9'))
#define CNT_KWORDS (27)

/* Types to check for function is_type */
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

/* Opcode numbers */
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

/*
 * @breif Checks if there are too many commas or commas one after anoher
 *
 * @param opcode a number which represents the opcode
 * @param arguments the arguments for the opcode
 * @param data
 *
 * @retrurn if the commas in the opcode are valid
 */
int comma_check(int opcode, char *arguments, int data);

/*
 * @breif Checks a token by type
 *
 * @param token A given token
 * @param type Type from enum
 *
 * @return true:1 , false:0
 */
int is_type(char *token, int type);

/**
 * @brief Parse a line to a create a word representing the opcode
 *
 * @param tok The opcode
 * @param line The arguments
 * @param parse Pointer to the parsed word
 * @param line_index Index of the current line
 * @param fname Name of the current file
 *
 * @return int The word representing the opcode
 */
int parse_code(char *tok, char *line, unsigned int *parse,
               unsigned int line_index, char *fname);

/*
 * @breif Finds the opcode representetive number
 *
 * @param tok the token (as string)
 * @return the number which represent the opcode
 */
int find_opcode(char *tok);

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
               unsigned int line_index, char *fname);

/**
 * @breif checks if a given label is valid
 *
 * @param label string of a label
 * @return if the label is valid
 */
int is_valid_label(char *label);

/**
 * @brief Parse an operand to a create a word representing the operand
 * 
 * @param args The operand
 * @param operand_num Number of operands
 * @param parse Pointer to the parsed word
 * @param first_register Determines if the first operand is a register
 * @param line_index Index of the current line
 * @param fname Name of the current file
 * @return 1 if error
 */
int first_cycle_parse_operand(char * args,int operand_num, unsigned int * parse, unsigned int * first_register, int line_index,char * fname);

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
int second_cycle_parse_operand(char * args,int operand_num, unsigned int * parse, unsigned int * first_register, label_t **curr,int line_index,char * fname);


#endif
