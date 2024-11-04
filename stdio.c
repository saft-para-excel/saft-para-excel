#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <json-c/json.h>
typedef struct {
    char root_path[1024];
    char spool_dir[1024];
    char database[1024];
    char saft_path[1024];
} Paths;

char* generate_hash_from_datetime(int size, int start) {
    char datetime_str[100];
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(datetime_str, sizeof(datetime_str), "%Y-%m-%d %H:%M:%S", tm_info);

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)datetime_str, strlen(datetime_str), hash);

    char hashed_datetime[SHA256_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(&hashed_datetime[i * 2], "%02x", hash[i]);
    }

    if (size != -1) {
        hashed_datetime[start + size] = '\0';
        return strdup(&hashed_datetime[start]);
    }

    return strdup(hashed_datetime);
}

void save_json(const char* file_path, json_object *d) {
    FILE *f = fopen(file_path, "w");
    if (f == NULL) {
        perror("Error opening file");
        return;
    }
    const char *json_str = json_object_to_json_string(d);
    fprintf(f, "%s", json_str);
    fclose(f);
}


void load_json(const char *file_path) {
    FILE *f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "Erro 335: %s\n", basename(file_path));
        exit(1);
    }
    // JSON loading logic would go here
    fclose(f);
}

Paths get_paths() {
    Paths d;
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    strcpy(d.root_path, cwd);
    d.root_path[strrchr(d.root_path, '/') - d.root_path] = '\0'; // Truncate to parent directory
    snprintf(d.spool_dir, sizeof(d.spool_dir), "%s/SPOOL/", d.root_path);
    snprintf(d.saft_path, sizeof(d.saft_path), "%s/SAFT/", d.root_path);
    strcpy(d.database, d.saft_path);
    return d;
}
