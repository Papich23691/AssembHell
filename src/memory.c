#include "memory.h"
#include "parse.h"
#include <string.h>

#define REGISTER_NUM 2
#define REGISTER_SIZE 3
#define SOURCE_REGISTER 128
#define DEST_REGISTER 4
#define START 100

int update_code(int run, char *tok, char *line_s, unsigned int line_index, char *fname, unsigned int *code, label_t **labels)
{
    unsigned int parse = 0, i = 0, reg = 0;
    char *args = line_s;
    label_t **curr = labels;

    if (!run)
    {
        if (parse_code(tok, line_s, &parse, line_index, fname)) /* error */
            return 1;
        code[IC] = parse;
        ++IC;
        i = find_opcode(tok);
        if (i == OPCODE_NUM)
        {
            add_front(&error_list, line_index, fname, "Unknown opcode");
            return 1;
        }
        if (comma_check(i, args, 0))
        {
            add_front(&error_list, line_index, fname, "Inappropriate use of commas");
            return 1;
        }
        parse = 0;
        args = strtok(args, " , ");
        /*////////////////////////////////////////////////////////////////// opcode stuff ^^^^^^^^^^^^^^^^^^^^^ */
        if (i <= SUB || i == LEA)
        {
            if (!args)
            {
                add_front(&error_list, line_index, fname, "Not enough arguments");
                return 1;
            }
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
            else if (!is_type(args, LABELN))
            {
                add_front(&error_list, line_index, fname, "Unknown argument");
                return 1;
            }
            /*************************** first argument ^^^^^^ *********************************************/
            args = strtok(NULL, " , ");
            if (!args)
            {
                add_front(&error_list, line_index, fname, "Not enough arguments");
                return 1;
            }
            if (!reg)
            {
                code[IC] = parse;
                parse = 0;
                ++IC;
            }
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
            {
                if (reg)
                {
                    code[IC] = parse;
                    ++IC;
                }
                parse = 0;
                parse += atoi(args) * DEST_REGISTER;
            }
            else if (is_type(args, LABELN))
            {
                if (reg)
                {
                    code[IC] = parse;
                    ++IC;
                }
                parse = 0;
            }
            else
            {
                add_front(&error_list, line_index, fname, "Unknown argument");
                return 1;
            }
            /*************************** second argument ^^^^^^ *********************************************/
            code[IC] = parse;
            ++IC;
        }
        /*////////////////////////////////////////////////////////////////// 2 arguments ^^^^^^^ */
        else if (i == NOT || i == CLR || (LEA < i && i < RTS))
        {
            if (!args)
            {
                add_front(&error_list, line_index, fname, "Not enough arguments");
                return 1;
            }
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
            else if (!is_type(args, LABELN))
            {
                add_front(&error_list, line_index, fname, "Unknown argument");
                return 1;
            }
            code[IC] = parse;
            ++IC;
        }
        /*////////////////////////////////////////////////////////////////// 1 argument ^^^^^^^^ */
        args = strtok(NULL, " , ");
        if (args)
        {
            add_front(&error_list, line_index, fname, "Extraneous text after end of command");
            return 1;
        }
    }
    else
    {
        i = find_opcode(tok);
        args = strtok(args, " , ");
        ++IC;
        if (i <= SUB || i == LEA)
        {
            if (is_type(args, LABELN))
            {
                while (*curr)
                {
                    if (!strcmp(args, (*curr)->name) && (*curr)->type == EXTERNL)
                    {
                        add_label(EXTERNL, args, IC + START, &ext);
                        parse += 1;
                        break;
                    }
                    else if (!strcmp(args, (*curr)->name))
                    {
                        parse += RELOCATION;
                        parse += (*curr)->address * DEST_REGISTER;
                        break;
                    }
                    curr = &(*curr)->next;
                }
                if (!*curr)
                {
                    add_front(&error_list, line_index, fname, "Unkown label");
                    return 1;
                }
                code[IC] = parse;
            }
            if (!is_type(args, REGISTER))
                ++IC;
            parse = 0;
            args = strtok(NULL, " , ");
            curr = labels;
            if (is_type(args, LABELN))
            {
                while (*curr)
                {
                    if (!strcmp(args, (*curr)->name) && (*curr)->type == EXTERNL)
                    {
                        add_label(EXTERNL, args, IC + START, &ext);
                        parse += 1;
                        break;
                    }
                    else if (!strcmp(args, (*curr)->name))
                    {
                        parse += RELOCATION;
                        parse += (*curr)->address * DEST_REGISTER;
                        break;
                    }
                    curr = &(*curr)->next;
                }
                if (!*curr)
                {
                    add_front(&error_list, line_index, fname, "Unkown label");
                    return 1;
                }
                ++IC;
                code[IC] = parse;
            }
            else if (!is_type(args, REGISTER))
                ++IC;
            ++IC;
            /*TODO second run*/
        }
        else if (i == NOT || i == CLR || (LEA < i && i < RTS))
        {
            if (is_type(args, LABELN))
            {
                while (*curr)
                {
                    if (!strcmp(args, (*curr)->name) && (*curr)->type == EXTERNL)
                    {
                        add_label(EXTERNL, args, IC + START, &ext);
                        parse += 1;
                        break;
                    }
                    else if (!strcmp(args, (*curr)->name))
                    {
                        parse += RELOCATION;
                        parse += (*curr)->address * DEST_REGISTER;
                        break;
                    }
                    curr = &(*curr)->next;
                }
                if (!*curr)
                {
                    add_front(&error_list, line_index, fname, "Unkown label");
                    return 1;
                }
                code[IC] = parse;
            }
            ++IC;
        }
        return 0;
        /*TODO second run*/
    }
    return 0;
}

int update_data(char *tok, char *line, unsigned int *data, unsigned int line_index, char *fname)
{
    char *args = duplicate_string(line);
    char *start_of_args = args;
    unsigned int parse = 0, i, argument = 0;

    if (!strcmp(tok, ".data"))
    {
        args = strtok(args, " , ");
        while (args)
        {
            if (parse_data(args, NUM_DATA, &parse, line_index, fname))
                return 1;
            data[DC] = parse;
            DC++;
            ++argument;
            args = strtok(NULL, " , ");
        }
        if (comma_check(OPCODE_NUM, line, argument))
        {
             add_front(&error_list, line_index, fname, "Inappropriate use of commas");
            return 1;
        }
    }
    else
    {
        if (line[0] != '"')
        {
            return 1;
        }
        for (i = 1; i < strlen(args) - 1 && args[i] != '"'; i++)
        {
            parse_data(args + i, CHAR_DATA, &parse, line_index, fname);
            data[DC] = parse;
            DC++;
        }
        if (i == strlen(args) - 1 && args[i] != '"')
        {
            add_front(&error_list, line_index, fname, "Missing a closing \"");
            return 1;
        }
        args = strtok(args + i + 1, " ");
        if (args)
        {
            add_front(&error_list, line_index, fname, "Extraneous text after end of command");
            return 1;
        }
        data[DC] = 0;
        DC++;
    }

    free(start_of_args);
    return 0;
}

void add_label(int type, char *name, int address, label_t **labels)
{
    label_t *new_node = (label_t*)malloc(sizeof(label_t));

    new_node->type = type;
    new_node->name = duplicate_string(name);
    new_node->address = address;
    new_node->next = NULL;

    if (*labels == NULL) {
        *labels = new_node;
    }
    else if ((*labels)->next == NULL) {
        (*labels)->next = new_node;
    }
    else
    {
        label_t *current = *labels;
        while (true) {
            if(current->next == NULL)
            {
                current->next = new_node;
                return;
            }
            current = current->next;
        };
    }
}

int add_data_label(unsigned int line_index, char *fname, char *name, label_t **labels)
{
    label_t **current_node = labels;

    while (*current_node)
    {
        if (!strcmp((*current_node)->name, name))
        {
            add_front(&error_list, line_index, fname, "Illegal label name");
            return 1;
        }
        current_node = &(*current_node)->next;
    }
    if (!is_type(name, LABELN))
    {
        printf("%s\n", name);

        add_front(&error_list, line_index, fname, "Illegal label name");
        return 1;
    }
    add_label(DATAL, name, DC + START, labels);
    return 0;
}

int add_extern_label(unsigned int line_index, char *fname, char *name, label_t **labels)
{
    label_t **current_node = labels;
    char *label = duplicate_string(name);

    while (*current_node)
    {
        if (!strcmp((*current_node)->name, name))
        {
            add_front(&error_list, line_index, fname, "Illegal label name");
            return 1;
        }
        current_node = &(*current_node)->next;
    }
    if (!is_type(label, LABELN))
    {
        add_front(&error_list, line_index, fname, "Illegal label name");
        return 1;
    }
    add_label(EXTERNL, label, 0, labels);
    return 0;
}

int add_code_label(unsigned int line_index, char *fname, char *name, label_t **labels)
{
    label_t **current_node = labels;

    if (!is_type(name, LABELN))
    {
        add_front(&error_list, line_index, fname, "Illegal label name");
        return 1;
    }
    while (*current_node)
    {
        if (!strcmp((*current_node)->name, name))
        {
            add_front(&error_list, line_index, fname, "Illegal label name");
            return 1;
        }
        current_node = &(*current_node)->next;
    }
    add_label(CODEL, name, IC + START, labels);
    return 0;
}

int update_entry(unsigned int line_index, char *fname, char *name, label_t **labels)
{
    label_t **current_node = labels;
    name = strtok(name, " ");
    while (*current_node)
    {
        if (!strcmp((*current_node)->name, name))
        {
            (*current_node)->type = ENTRYL;
            break;
        }
        current_node = &(*current_node)->next;
    }
    if (!(*current_node))
    {
        add_front(&error_list, line_index, fname, "Unkown label");
        return 1;
    }
    name = strtok(NULL, " ");
    if (name)
    {
        add_front(&error_list, line_index, fname, "Extraneous text after end of command");
        return 1;
    }
    return 0;
}

void delete_labels_list(label_t **root) {
  label_t *curr = *root, *tmp = NULL;

  while (curr != NULL) {
    tmp = curr;
    curr = curr->next;
    free(tmp->name);
    free(tmp);
  }

    *root = NULL;
}