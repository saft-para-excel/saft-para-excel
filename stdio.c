#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <openssl/sha.h>
#include <json-c/json.h>

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

