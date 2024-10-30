#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#define SPOOL_DIR "path_to_spool_dir/"
#define SAFT_PATH "path_to_saft_path/"
#define LOCK_DIR "LOCK"

void main(); // Forward declaration of main function

void spool_reader() {
    struct dirent *entry;
    DIR *dp = opendir(SPOOL_DIR);
    
    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, LOCK_DIR) == 0) {
                printf("LOCK found\n");
            } else {
                printf("%s%s -> %s%s\n", SPOOL_DIR, entry->d_name, SAFT_PATH, entry->d_name);
                
                char source[256];
                char destination[256];
                snprintf(source, sizeof(source), "%s%s", SPOOL_DIR, entry->d_name);
                snprintf(destination, sizeof(destination), "%s%s", SAFT_PATH, entry->d_name);
                
                rename(source, destination); // Move the directory
                
                // Assuming gl.WORKING_DIR and gl.JSON_FILE are global variables
                extern char *WORKING_DIR;
                extern char JSON_FILE[256];
                WORKING_DIR = entry->d_name;
                snprintf(JSON_FILE, sizeof(JSON_FILE), "%s%s/data.json", SAFT_PATH, WORKING_DIR);
                
                main(); // Call main function
            }
        }
    }
    closedir(dp);
}

