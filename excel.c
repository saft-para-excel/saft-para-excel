#include <stdio.h>
#include <string.h>
#include <xls_name.h>

char* make_xls_file_name();

void save_workbook() {
    // Grava a folha mas calcula o nome antes
    char xls_file_name[256]; // Adjust size as needed
    strcpy(xls_file_name, make_xls_file_name());
    gl.GLOBAL_DICT["excel_file"] = xls_file_name; // This line assumes a way to set a string in a dictionary
    gl.workbook.save(xls_file_name); // This line assumes a way to save the workbook
}

