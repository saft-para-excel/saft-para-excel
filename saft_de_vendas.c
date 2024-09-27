#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <read_file.h>
#include <watchdog.h>
void saft_vendas() {
    /*
        A função que é chamada pelo watchdog
    */
    double total_execution_time = 0;
    clock_t start_time = clock();
    bool flag = true;
    bool responce = saft_setup(); 

    if (responce) { // WORKING_FILES != []:
        printf("F: %6s\n", WORKING_DIR); 
        for (int i = 0; i < WORKING_FILES_COUNT; i++) { 
            char *n = WORKING_FILES[i]; 
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
            printf("%s OK\n", basename(n)); 
        }
    }
}

