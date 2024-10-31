#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#define SPOOL_DIR "path_to_spool_dir" 
#define SAFT_PATH "path_to_saft_path" 
#define LOCK_DIR "LOCK"
#define JSON_FILE "data.json"

void main(); 

void spool_reader(char *command[]) {
    int wait_seconds;
    if (command[1] != NULL) {
        wait_seconds = atoi(command[1]);
    } else {
        wait_seconds = 10;
    }
    printf("seconds: %d\n", wait_seconds);
    
    struct stat buffer;
    int stop_command = (stat(SPOOL_DIR "/stop", &buffer) == 0);

    while (!stop_command) {
        DIR *d;
        struct dirent *dir;
        d = opendir(SPOOL_DIR);
        if (d) {
            while ((dir = readdir(d)) != NULL) {
                if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) {
                    if (strcmp(dir->d_name, LOCK_DIR) == 0) {
                        printf("LOCK found\n");
                    } else {
                        printf("COPY: %s/%s\n", SPOOL_DIR, dir->d_name);
                        char source[256];
                        char destination[256];
                        snprintf(source, sizeof(source), "%s/%s", SPOOL_DIR, dir->d_name);
                        snprintf(destination, sizeof(destination), "%s/%s", SAFT_PATH, dir->d_name);
                        rename(source, destination); // Move file
                        main();
                    }
                }
            }
            closedir(d);
        }
        sleep(wait_seconds);
        stop_command = (stat(SPOOL_DIR "/stop", &buffer) == 0);
    }
    printf("STOP by command\n");
}

