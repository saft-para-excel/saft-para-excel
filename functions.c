#include <stdio.h>
#include <stdlib.h>
// funcões genericas como no PHP

int remove_ns(const char *file_name) {
    char command[256];
    snprintf(command, sizeof(command), "sed -i -e 's/ns://g' %s", file_name);
    int return_code = system(command);
    return return_code;
}

