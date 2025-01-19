#include <stdio.h>
#include <time.h>
#include <sys/stat.h>

void create_workbook();
void create_excel();
void save_workbook();
double seconds_to_hours(double seconds);

struct {
    char excel_file[256];
    long excel_file_size;
    double to_excel_time;
} GLOBAL_DICT;

void e_factura_to_excel(const char *file_xls) {
    double total_execution_time = 0;
    clock_t start_time = clock();
    
    create_workbook();
    create_excel();
    save_workbook();
    
    struct stat st;
    stat(GLOBAL_DICT.excel_file, &st);
    GLOBAL_DICT.excel_file_size = st.st_size;
    GLOBAL_DICT.to_excel_time = seconds_to_hours(total_execution_time);
}

