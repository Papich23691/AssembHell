#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "memory.h"
#include "files.h"

#define START 100

extern int DC, IC;

int main(int argc, char *argv[])
{
    char *name = NULL, *line_s = NULL, *tok = NULL, *label = NULL;
    node_t *data = NULL, *code = NULL;
    unsigned int line_index = 0;
    int i, j = 0, flag = 0;
    label_t *labels = NULL;
    FILE *file = NULL;

    for (i = 0; argv[i]; i++)
    {
        /* Initializtions */
        name = argv[i] error = 0;
        DC = 0;
        IC = 0;

        strcat(name, ".as");

        file = fopen(name, "r");
        data = (node_t *)malloc(sizeof(node_t *));
        code = (node_t *)malloc(sizeof(node_t *));
        labels = (label *)malloc(sizeof(label *));

        while (fgets(line, sizeof(line), file))
        {
            if (line[0] != ';' || line[0] != '\n')
            {
                tok = strtok(line, " ");
                flag = 0;
                label = "";

                if (is_type(tok, LABEL))
                {
                    flag = 1;
                    strncpy(label, tok, strlen(tok) - 1);
                    tok = strtok(NULL, " ");
                }
                if (is_type(tok, DATA))
                {
                    if (flag)
                        error += add_data_label(label, labels);
                    error += update_data(tok, line, data);
                    continue;
                }
                else if (is_type(tok, EXTERN))
                {
                    error += add_extern_label(label);
                    continue;
                }
                else if (is_type(tok, CODE))
                {
                    if (flag)
                        error += add_code_label(label);
                    error += update_code(0, tok, line, line, );
                }

                ++line_index;
            }
        }

        fclose(file);

        if (error)
        {
            free(labels);
            free(code);
            free(data);
            return 1;
        }

        label *point = (label *)malloc(sizeof(label *));
        point = labels;
        while (point)
        {
            if (!strcmp(point->type, "data"))
                point->value += IC;
            point = point->next;
        }
        free(point);

        IC = 0;
        line = 0;
        file = fopen(name, "r");

        while (fgets(line, sizeof(line), file))
        {
            if (line[0] != ';' && line[0] != '\n')
            {
                char *tok = strtok(line, " ");
                if (is_type(tok, DATA) || is_type(tok, EXTERN))
                    continue;
                if (is_type(tok, LABEL))
                    tok = strtok(NULL, " ");
                if (is_type(tok, ENTRY))
                {
                    error += update_entry(line);
                    continue;
                }
                if (is_type(tok, CODE))
                    error += update_code(1, tok, line, code, names);
                line++;
            }
        }
        
        if (error)
            return 1;
        
        create_files(code, data, labels, argv[i]);
        
        free(labels);
        free(code);
        free(data);
    }
}