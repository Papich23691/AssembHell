#include "files.h"
extern label_t *ext;
const unsigned char base64_digset[B64_DIG_LEN] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char *bin_to_64(int value, char *conversion)
{
  conversion[1] = base64_digset[value & SIX_BITS];
  value >>= BIT_MOVE;
  conversion[0] = base64_digset[value & SIX_BITS];

  return conversion;
}
void create_error_file(err_node_t *head)
{
  FILE *fp;
  err_node_t *current_node = head;
  fp = fopen("errors.txt", "w");
  if (fp)
  {
    while (current_node)
    {
      fprintf(fp, "Error\n%s.\nFile: %s\tLine: %u\n", current_node->desc, current_node->fname, current_node->line);
      current_node = current_node->next;
    }
  }
}

void create_files(unsigned int code[1024], unsigned int data[1024], label_t *labels, char *name)
{
  FILE *fp;
  char con[3] = { 0 };
  int flag=0;
  int i;
  label_t *curr = labels;
  char *fn = (char *)malloc(sizeof(name) + 4);
  sprintf(fn, "%s.ob", name);
  fp = fopen(fn, "w+");
  fprintf(fp, "%d %d\n", IC, DC);
  for (i = 0; i < IC; i++)
  {
    fprintf(fp, "%s\n", bin_to_64(code[i], con));
  }
  for (i = 0; i < DC; i++)
  {
    fprintf(fp, "%s\n", bin_to_64(data[i], con));
  }
  fclose(fp);
  sprintf(fn, "%s.ent", name);
  fp = fopen(fn, "w+");
  while (curr)
  {
    if (curr->type == ENTRYL){
      flag=1;
      fprintf(fp, "%s\t%d\n", curr->name, curr->address);
    }
    curr = curr->next;
  }
  fclose(fp);
  if (!flag){
    remove(fn);
  }
  flag=0;
  sprintf(fn, "%s.ext", name);
  fp = fopen(fn, "w+");
  curr = ext;
  while (curr)
  {
    flag=1;
    fprintf(fp, "%s\t%d\n", curr->name, curr->address);
    curr = curr->next;
  }
  fclose(fp);
  if (!flag){
    remove(fn);
  }
}
