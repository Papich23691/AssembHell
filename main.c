#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parse.h"
#include "memory.h"
#include "files.h"

#define START 100

int error, DC, IC;

int main(int argc, char *argv[])
{
    int i;
    for (i = 0; argv[i]; i++)
    {
        error = 0;
        node *Data = (node *)malloc(sizeof(node *));
        node *Code = (node *)malloc(sizeof(node *));
        label *Labels = (label *)malloc(sizeof(label *));
        DC = 0;
        IC = 0;
        char * name = argv[i];
        strcat(name, ".as");
        FILE *file = fopen(name, "r");
        char *line;
        while (fgets(line, sizeof(line), file))
        {
            char *tok = strtok(line, " ");
            int flag = 0;
            char *label = "";
            if (is_type(tok, LABEL))
            {
                flag = 1;
                strncpy(label, tok,strlen(tok)-1);
                tok = strtok(NULL, " ");
            }
            if (is_type(tok, DATA))
            {
                if (flag)
                    error += add_data_label(label, DC, Labels);
                error += update_data(tok, line, &DC, Data);
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
                    error += add_code_label(label, IC);
                error += update_code(0, tok, line, &IC, Code);
            }
        }
        fclose(file);
        if (error)
        {
            free(Labels);
            free(Code);
            free(Data);
            return 1;
        }
        int j;
        label *point = (label *)malloc(sizeof(label *));
        point = Labels;
        while (point)
        {
            if (!strcmp(point->type, "data"))
                point->value += IC;
            point = point->next;
        }
        free(point);
        IC = 0;
        file = fopen(name, "r");
        while (fgets(line, sizeof(line), file))
        {
            char *tok = strtok(line, " ");
            if (is_type(tok, LABEL))
                tok = strtok(NULL, " ");
            if (is_type(tok, DATA) || is_type(tok, EXTERN))
                continue;
            if (is_type(tok, ENTRY))
            {
                error += update_entry(line);
                continue;
            }
            if (is_type(tok, CODE))
                error += update_code(1, tok, line, &IC, Code);
        }
        if (error)
            return 1;
        create_files(Code, Data, Labels, argv[i]);
        free(Labels);
        free(Code);
        free(Data);
    }
}