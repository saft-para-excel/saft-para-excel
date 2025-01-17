#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>
#include "setup_files.h"
#include "stock.h"
#include "log_crud.h"
#include "excel.h"
#include "smtpmail.h"
#include "stdio_utils.h"
#include "global.h"

void stock_integration() {
    /*
        a função que é chamada pelo watchdog
    */
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
                bool ready = stock_to_database(n);
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

        if (flag) {  // a converte para excel
            stock_count_to_excel();
            mail_gateway();
        } else {
            printf("\nError on reading HEADER SAF-t file #46"
                   "\ndá sempre este erro enquanto o ready não for validado\n");
            write_error("Error on reading STOCK file #48");
        }
    } else {
        printf("Houve um erro\n");
    }
    
    clock_t end_time = clock();
    total_execution_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("%14s\n", format_seconds_to_hours(total_execution_time));
    printf("%s\n", "----------------------------------------");
    GLOBAL_DICT["total_time"] = seconds_to_hours(total_execution_time);
}

