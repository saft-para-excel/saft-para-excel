pp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

void spool_reader(char *command[]) {
    // lê o spool e processa o ficheiros
    gl.GLOBAL_DICT["linting"] = true;
    stdio_get_paths();
    
    int wait_seconds;
    if (command[1] != NULL) {
        wait_seconds = atoi(command[1]);
    } else {
        wait_seconds = 10;
    }
    printf("seconds: %d\n", wait_seconds);
    
    bool stop_command = access(gl.SPOOL_DIR "/stop", F_OK) == 0;
    while (!stop_command) {
        char *oldest = NULL;
        char path[256];
        strcpy(path, gl.SPOOL_DIR);
        chdir(path);
        
        struct dirent **namelist;
        int n = scandir(".", &namelist, NULL, alphasort);
        if (n < 0) {
            perror("scandir");
            return;
        } else {
            time_t most_recent_time = 0;
            for (int i = 0; i < n; i++) {
                if (namelist[i]->d_type == DT_REG) {
                    struct stat file_stat;
                    stat(namelist[i]->d_name, &file_stat);
                    if (file_stat.st_mtime > most_recent_time) {
                        oldest = namelist[i]->d_name;
                        most_recent_time = file_stat.st_mtime;
                    }
                }
                free(namelist[i]);
            }
            free(namelist);
        }

        if (oldest != NULL) {
            gl.WORKING_DIR = oldest;
            printf("%14s: %s\n", "FOUND:", gl.WORKING_DIR);
            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            printf("%14s: %02d-%02d-%04d %02d:%02d\n", "DATE:", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900, t->tm_hour, t->tm_min);
            char source[512];
            snprintf(source, sizeof(source), "%s/%s", gl.SPOOL_DIR, oldest);
            char destination[512];
            snprintf(destination, sizeof(destination), "%s/%s", gl.SAFT_PATH, oldest);
            rename(source, destination);
            snprintf(gl.JSON_FILE, sizeof(gl.JSON_FILE), "%s/%s/data.json", gl.SAFT_PATH, gl.WORKING_DIR);
            main();
        } else {
            sleep(wait_seconds);
        }

        stop_command = access(gl.SPOOL_DIR "/stop", F_OK) == 0;
    }
    printf("STOP by command\n");
}

