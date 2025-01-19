#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>
#include <ftp.h> 
void log_write(const char *file_name, const char *content) {
    FILE *myfile = fopen(file_name, "a");
    if (myfile != NULL) {
        fprintf(myfile, "%s", content);
        fclose(myfile);
    }
}

int file_ok(const char *file) {
    return access(file, F_OK) == 0;
}

void make_dir(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        if (mkdir(path, 0777) == -1) {
            perror("mkdir failed");
        }
    }
}

void postgresql(const char *disco, char **db_list, int db_count) {
    printf("Fazendo backup do Postgresql\n");
    setenv("PGUSER", "db_admin", 1);
    setenv("PGPASSWORD", "ElectRic_Light_orcHesTra_tIME", 1);
    char file_list[100][256]; 
    int file_count = 0;
    make_dir(disco);
    for (int i = 0; i < db_count; i++) {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char file_name[256];
        snprintf(file_name, sizeof(file_name), "%s%s_%04d-%02d-%02d_%02d%02d.sql", disco, db_list[i], tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour * 100 + tm.tm_min);
        char exec_string[512];
        snprintf(exec_string, sizeof(exec_string), "pg_dump -h localhost -p 5432 -U root -F p -f %s %s", file_name, db_list[i]);
        system(exec_string);
        strcpy(file_list[file_count++], file_name);
    }
    send_ftp(file_list, file_count);
}

void send_ftp(char file_list[][256], int file_count) {
    printf("Start Sending\n");
    for (int i = 0; i < file_count; i++) {
        FILE *x_file = fopen(file_list[i], "rb");
        if (x_file != NULL) {
            char *file_name = strrchr(file_list[i], '/') ? strrchr(file_list[i], '/') + 1 : file_list[i];
            printf("%-40s OK\n", file_name);
            fclose(x_file);
        } else {
            printf("ERRO: %s\n", strerror(errno));
        }
    }
    printf("OK\n");
}

