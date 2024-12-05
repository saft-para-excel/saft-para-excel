#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void write_error_to_file() {
    // cria um ficheiro do erro
    char *list_of_strings[100]; // Assuming a maximum of 100 keys for simplicity
    int index = 0;

    
    for (int i = 0; i < gl.GLOBAL_DICT_SIZE; i++) {
        char *key = gl.GLOBAL_DICT_KEYS[i];
        char *value = gl.GLOBAL_DICT[key];
        list_of_strings[index] = malloc(strlen(key) + strlen(value) + 10); // Allocate memory for the string
        sprintf(list_of_strings[index], "%s : %s", key, value);
        index++;
    }

    char filepath[256];
    sprintf(filepath, "%s/%s/ERROR.txt", gl.SAFT_PATH, gl.GLOBAL_DICT["working_dir"]);

    FILE *f = fopen(filepath, "w");
    if (f == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < index; i++) {
        fprintf(f, "%s\n", list_of_strings[i]);
        free(list_of_strings[i]); // Free allocated memory
    }

    fclose(f);
}

