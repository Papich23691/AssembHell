#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "memory.h"
#include "files.h"
#include "error.h"

extern int DC, IC;
extern err_node_t *error_list;

int main(int argc, char *argv[])
{
    FILE *file = NULL;
    char *name = NULL, *cmd = NULL, *label = NULL, *args = NULL;
    char line_s[256];
    unsigned int line_index = 0;
    int i, error, flag = 0;
    label_t *labels = NULL;
    label_t **point = &labels;
    error_list = NULL;

    for (i = 1; i < argc; ++i)
    {
        /* Initializtions */
        name = argv[i];
        error = 0;
        DC = 0;
        IC = 0;
        file = fopen(name, "r");

        if (file == NULL)
        {
            printf("Error opening file\n");
            continue;
        }
        
        while (fgets(line_s, sizeof(line_s), file))
        {
            printf("%d:%d\t%s\n",line_index,error,line_s);
            ++line_index;
            if (line_s[0] != ';' && line_s[0] != '\n')
            {
                cmd = strtok(line_s, " ");
                args = strtok(NULL, "\n");
                flag = 0;
                label = "";
                if (is_type(cmd, LABEL))
                {
                    flag = 1;
                    cmd[strlen(cmd) - 1] = '\0';
                    label = duplicate_string(cmd);
                    cmd = strtok(args, " ");
                    args = strtok(NULL, "\n");
                }
                if (is_type(cmd, DATA))
                {
                    if (flag)
                        error += add_data_label(line_index, name, label, &labels);
                    error += update_data(cmd, args, data, line_index, name);
                    continue;
                }
                else if (is_type(cmd, EXTERN))
                {
                    error += add_extern_label(line_index, name, args, &labels);
                    continue;
                }
                else if (is_type(cmd, CODE))
                {
                    if (flag)
                        error += add_code_label(line_index, name, label, &labels);
                    error += update_code(0, cmd, args, line_index, name, code, &labels);
                }
            }
        }

        fclose(file);
        
        if (error)
        {
            create_error_file(error_list);
            continue;
        }
        
        while (*point)
        {
            if ((*point)->type == DATAL)
                (*point)->address += IC;
            point = &(*point)->next;
        }
        
        IC = 0;
        file = fopen(name, "r");
        
        if (file == NULL)
        {
            printf("Error opening file\n");
            continue;
        }
        
        line_index = 0;
        while (fgets(line_s, sizeof(line_s), file))
        {
            ++line_index;
            if (line_s[0] != ';' && line_s[0] != '\n')
            {
                cmd = strtok(line_s, " ");
                args = strtok(NULL, "\n");
                if (is_type(cmd, DATA) || is_type(cmd, EXTERN))
                    continue;
                if (is_type(cmd, LABEL))
                {
                    cmd = strtok(args, " ");
                    args = strtok(NULL, "\n");
                }
                if (is_type(cmd, ENTRY))
                {
                    error += update_entry(line_index, name, args, &labels);
                    continue;
                }
                if (is_type(cmd, CODE))
                {
                    error += update_code(1, cmd, args, line_index, name, code, &labels);
                }
            }
        }

        if (error)
        {
            printf("error\n");
            create_error_file(error_list);
            continue;
        }

        name = strtok(name, ".");
        create_files(code, data, labels, argv[i]);
        free(labels);
    }

    delete_errors_list(&error_list);
    

    return 0;
}
