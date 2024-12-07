#include <stdio.h>
#include <stdlib.h>
// funcões genericas como no PHP

int remove_ns(const char *file_name) {
    char command[256];
    snprintf(command, sizeof(command), "sed -i -e 's/ns://g' %s", file_name);
    int return_code = system(command);
    return return_code;
}


int lint_file(const char *file_name) {
    char backup_file[256];
    snprintf(backup_file, sizeof(backup_file), "%s.bak", file_name);
    
    // Backup file
    if (rename(file_name, backup_file) != 0) {
        perror("Error renaming file");
        return -1;
    }

    // Prepare command for linting
    char command[512];
    snprintf(command, sizeof(command), "xmllint %s --pretty 1 > %s", backup_file, file_name);

    // Execute command
    int return_code = system(command);
    return return_code;
}

