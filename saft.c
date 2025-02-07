
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "setup_files.h"
#include "sql_oracle.h"
#include "saft.h"
#include "log_crud.h"
#include "update_db.h"
#include "excel.h"
#include "smtpmail.h"
#include "global.h"
#include "stdio_utils.h"

typedef struct {
    char* key;
    char* value;
} KeyValuePair;

typedef struct {
    KeyValuePair* pairs;
    int size;
} Dictionary;

Dictionary* tag_to_dict(char* n) {
    return NULL;
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
struct GlobalDict {
    char tax_registration_number[50];
    char company_name[100];
    char start_date[20];
    char end_date[20];
};

struct GlobalDict gl;

char* saft_subject() {
    static char a[300];
    snprintf(a, sizeof(a), "Excel do SAF-t de NIF:%s %s (%s a %s)", 
             gl.tax_registration_number, 
             gl.company_name, 
             gl.start_date, 
             gl.end_date);
    return a;
}


void sales_documents_integration() {
    printf("Integra documentos de venda existentes no ficheiro SAF_t na base de dados Oracle");
    double total_execution_time = 0;
    clock_t start_time = clock();
    bool flag = true;
    bool responce = saft_setup();

    if (responce) {
        init_database();
        for (int i = 0; i < WORKING_FILES_COUNT; i++) {
            const char *n = WORKING_FILES[i];
            bool a = read_file(n);
            if (a) {
                bool ready = saft_to_database(n);
                if (!ready) {
                    printf("Error on integration from SAF-t to DBb #51\n");
                    flag = false;
                    break;
                }
            } else {
                printf("Error on reading SAF-t file #71\n");
                write_error("Error on reading SAF-t file #71");
                flag = false;
            }
            printf("%14s\n", basename(n));
        }
        if (GLOBAL_DICT["products"] == 0) {
            create_from_sales();
        }
        update_customers();
        update_products();
        if (flag) {
            database_to_excel();
            mail_gateway();
        } else {
            printf("\nError on reading HEADER SAF-t file #68\n");
            write_error("Error on reading SAF-t file #57");
        }
    } else {
        printf("Houve um erro\n");
    }
    clock_t end_time = clock();
    total_execution_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("%14s\n", format_seconds_to_hours(total_execution_time));
    printf("%s\n", "---------TERMINAO------------");
    GLOBAL_DICT["total_time"] = seconds_to_hours(total_execution_time);
    log_update(GLOBAL_DICT);
}

