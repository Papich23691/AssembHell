#ifndef __H_MEMORY
#define __H_MEMORY

#include "error.h"
#include <stdbool.h>
#include <stdio.h>

#define NUM_DATA (0)
#define CHAR_DATA (1)
#define RELOCATION 2
#define WORD_SIZE (12)
#define MEMORY_SIZE 1024

/* Label type */
enum labels
{
  DATAL = 0,
  CODEL,
  ENTRYL,
  EXTERNL
};

typedef struct node
{
  int type;
  char *name;
  int address;
  struct node *next;
} label_t;
/* Memory */
unsigned int code[MEMORY_SIZE];
unsigned int data[MEMORY_SIZE];
/* Memory counters */
int DC, IC;
/* Error stack and linked list of extern operands */
err_node_t *error_list;
label_t *ext;

/**
 * @brief Adds label of type data
 * 
 * @param line_index Index of current line
 * @param fname Name of current file
 * @param name Name of label
 * @param labels Head of linked list
 * @return int Return 1 if error
 */
int add_data_label(unsigned int line_index, char *fname, char *name,
                   label_t **labels);

/**
 * @brief Adds data words to memory
 * 
 * @param tok Data type
 * @param line Arguments
 * @param data Memory of data words
 * @param line_index Number of current line
 * @param fname Name of current file
 * @return int returns 1 if error
 */  
int update_data(char *tok, char *line, unsigned int *data,
                unsigned int line_index, char *fname);

/**
 * @brief Adds label of type extern
 * 
 * @param line_index Index of current line
 * @param fname Name of current file
 * @param name Name of label
 * @param labels Head of linked list
 * @return int Return 1 if error
 */
int add_extern_label(unsigned int line_index, char *fname, char *name,
                     label_t **labels);

/**
 * @brief Adds label of type code
 * 
 * @param line_index Index of current line
 * @param fname Name of current file
 * @param name Name of label
 * @param labels Head of linked list
 * @return int Return 1 if error
 */
int add_code_label(unsigned int line_index, char *fname, char *name,
                   label_t **labels);

/**
 * @brief Adds code words to memory
 * first cycle - adds code words except labels
 * second cycle - adds label code words
 * 
 * @param run 0 = first cycle , 1 = second cycle
 * @param tok The opcode
 * @param line_s The string of rest of the line
 * @param line_index Number of current line
 * @param fname Name of current file
 * @param code Memory of code words
 * @param labels Labels table
 * @return int returns 1 if error
 */
int update_code(int run, char *tok, char *line_s, unsigned int line_index,
                char *fname, unsigned int *code, label_t **labels);

/**
 * @brief Update each entry label in label table
 * 
 * @param line_index Index of current line
 * @param fname Name of current file
 * @param name Name of label
 * @param labels Head of linked list
 * @return int Return 1 if error
 */
int update_entry(unsigned int line_index, char *fname, char *name,
                 label_t **labels);
/**
 * @brief Adds node to linked list
 * 
 * @param type Label type
 * @param name Label name
 * @param address Label address
 * @param labels Head of linked list
 */
void add_label(int type, char *name, int address, label_t **labels);

/**
 * @brief Deletes linked lists from memory
 * 
 * @param root Root of linked list
 */
void delete_labels_list(label_t **root);

#endif
