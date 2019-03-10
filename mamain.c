#include "files.h"
#include "error.h"

#include <stdlib.h>

int main(void) {
    err_node_t *root = NULL;
    const char *fname = "Fuck.h", *desc = "LoL Descrition";
    add_front(&root, 15, fname, desc);
    add_front(&root, 15, fname, desc);
    add_front(&root, 15, fname, desc);
    add_front(&root, 15, fname, desc);
    add_front(&root, 15, fname, desc);
    add_front(&root, 15, fname, desc);

    display_list(root);
    delete_list(&root);
    return 0;
}
