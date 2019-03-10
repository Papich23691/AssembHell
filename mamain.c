#include <stdlib.h>
#include "files.h"


int main(void) {
	err_node_t *err = NULL;
	add_front(&err, 15, "Fuck.h", "Description");
	display_list(err);
	create_error_file(err);

	return 0;
}
