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
    unsigned int line_index = 0;
    int i,error, j = 0, flag = 0;
    label_t *labels = NULL;
    FILE *file = NULL;

    for (i = 0; argv[i]; i++)
    {
        /* Initializtions */
        name = argv[i];
        error = 0;
        DC = 0;
        IC = 0;

        strcat(name, ".as");

        file = fopen(name, "r");

        while (fgets(line_s, sizeof(line_s), file))
        {
            if (line_s[0] != ';' || line_s[0] != '\n')
            {
                tok = strtok(line_s, " ");
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
                    error += update_data(tok, line_s, data);
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
                    error += update_code(0, tok, line_s, line_index, name,code);
                }

                ++line_index;
            }
        }

        fclose(file);

        if (error){
            return 1;
            //create_error_file
        }

        /*label *point = (label *)malloc(sizeof(label *));
        point = labels;
        while (point)
        {
            if (!strcmp(point->type, "data"))
                point->value += IC;
            point = point->next;
        }
        free(point);
        */
        IC = 0;
        file = fopen(name, "r");

        while (fgets(line_s, sizeof(line_s), file))
        {
            if (line_s[0] != ';' && line_s[0] != '\n')
            {
                char *tok = strtok(line_s, " ");
                if (is_type(tok, DATA) || is_type(tok, EXTERN))
                    continue;
                if (is_type(tok, LABEL))
                    tok = strtok(NULL, " ");
                if (is_type(tok, ENTRY))
                {
                    error += update_entry(line_s);
                    continue;
                }
                if (is_type(tok, CODE))
                    error += update_code(1, tok, line_s, line_index, name,code);
                ++line_index;
            }
        }
        if (error){
            return 1;
            //create_error_file
        }
        create_files(code, data, labels, argv[i]);
        
        free(labels);
        free(code);
        free(data);
    }
}