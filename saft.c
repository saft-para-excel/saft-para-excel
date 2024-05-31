#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char* key;
    char* value;
} KeyValuePair;

typedef struct {
    KeyValuePair* pairs;
    int size;
} Dictionary;

Dictionary* tag_to_dict(char* n) {
    // Implement the tag_to_dict function to convert a string to a dictionary
    // This function is not provided in the input code
    return NULL;
}

void insert_header(Dictionary* header_dict) {
    // Implement the insert_header function to insert the header dictionary into the database
    // This function is not provided in the input code
}

void write_header_to_file(Dictionary* header_dict) {
    // Implement the write_header_to_file function to write the header dictionary to a file
    // This function is not provided in the input code
}

void log_insert_header() {
    // Implement the log_insert_header function to log the insertion of the header
    // This function is not provided in the input code
}

int get_header(int* file_pointers) {
    Dictionary* header_dict = malloc(sizeof(Dictionary));
    header_dict->pairs = NULL;
    header_dict->size = 0;

    for (int p_line = file_pointers[0]; p_line < file_pointers[1]; p_line++) {
        char* n = gl.SAFT_FILE_DATA[p_line];
        Dictionary* tag_dict = tag_to_dict(n);
        header_dict = realloc(header_dict, sizeof(Dictionary) * (header_dict->size + tag_dict->size));
        for (int i = 0; i < tag_dict->size; i++) {
            header_dict->pairs[header_dict->size + i] = tag_dict->pairs[i];
        }
        header_dict->size += tag_dict->size;
        free(tag_dict->pairs);
        free(tag_dict);
    }

    if (gl.GLOBAL_DICT == NULL || gl.GLOBAL_DICT->size == 0 || gl.GLOBAL_DICT->pairs[0].key == NULL || strcmp(gl.GLOBAL_DICT->pairs[0].key, "website") != 0) {
        Dictionary* website_dict = malloc(sizeof(Dictionary));
        website_dict->pairs = malloc(sizeof(KeyValuePair));
        website_dict->pairs[0].key = "website";
        website_dict->pairs[0].value = "";
        website_dict->size = 1;
        gl.GLOBAL_DICT = website_dict;
    }

    insert_header(header_dict);
    for (int i = 0; i < header_dict->size; i++) {
        gl.GLOBAL_DICT->pairs = realloc(gl.GLOBAL_DICT->pairs, sizeof(KeyValuePair) * (gl.GLOBAL_DICT->size + 1));
        gl.GLOBAL_DICT->pairs[gl.GLOBAL_DICT->size] = header_dict->pairs[i];
        gl.GLOBAL_DICT->size++;
    }
    log_insert_header();
    write_header_to_file(header_dict);

    free(header_dict->pairs);
    free(header_dict);

    return 1;
}

