#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void create_excel() {
    // Function to create a cell in Excel format
    char cell[3];
    int line = 1;
    int col = 0; 
    const char *colum_data[][3] = {
        {"A", "Diário", "15"},
        {"B", "Descrição do Diário", "15"},
        {"C", "Credito id", "15"},
        {"D", "Conta a Crédito", "15"},
        {"E", "Documento a Crédito de Origem", "15"},
        {"F", "credit_system_entry_date", "15"},
        {"G", "credit_description", "15"},
        {"H", "credit_amount", "15"},
        {"I", "debit_record_id", "15"},
        {"J", "debit_account_id", "15"},
        {"K", "debit_source_document_id", "15"},
        {"L", "debit_system_entry_date", "15"},
        {"M", "debit_description", "15"},
        {"N", "debit_amount", "15"},
        {"O", "account_id", "15"},
        {"P", "account_description", "15"},
        {"Q", "transaction_id", "15"},
        {"R", "period", "15"},
        {"S", "transaction_date", "15"},
        {"T", "source_id", "15"},
        {"U", "description", "15"},
        {"V", "doc_archival_number", "15"},
        {"W", "transaction_type", "15"},
        {"X", "gl_posting_date", "15"},
        {"Y", "customer_id", "15"},
        {"Z", "supplier_id", "15"},
        {"AA", "account_id", "15"},
        {"AB", "grouping_category", "15"},
        {"AC", "grouping_code", "15"},
        {"AD", "taxonomy_code", "15"}
    };

    // Create columns
    const char *sql = "SELECT journals.jornal_id AS 'Diário', "
                      "journals.description AS 'Descrição do Diário', "
                      "credit_record_id AS 'Credito id', "
                      "credit_account_id AS 'Conta a Crédito', "
                      "credit_source_document_id AS 'Documento a Crédito de Origem', "
                      "credit_system_entry_date, "
                      "credit_description, "
                      "credit_amount, "
                      "debit_record_id, "
                      "debit_account_id, "
                      "debit_source_document_id, "
                      "debit_system_entry_date, "
                      "debit_description, "
                      "debit_amount, "
                      "accounts.account_id, "
                      "accounts.account_description, "
                      "transaction_id, "
                      "period, "
                      "transaction_date, "
                      "source_id, "
                      "transactions.description, "
                      "doc_archival_number, "
                      "transaction_type, "
                      "gl_posting_date, "
                      "customer_id, "
                      "supplier_id, "
                      "accounts.account_id, "
                      "accounts.grouping_category, "
                      "accounts.grouping_code, "
                      "accounts.taxonomy_code "
                      "FROM transaction_lines "
                      "INNER JOIN accounts ON transactions.customer_id = accounts.account_id "
                      "INNER JOIN transactions ON transactions.transaction_hash = transaction_lines.transaction_hash "
                      "INNER JOIN journals ON transactions.journal_hash = journals.journal_hash "
                      "LEFT OUTER JOIN accounts a ON a.account_id = transaction_lines.debit_account_id "
                      "LEFT OUTER JOIN accounts b ON b.account_id = transaction_lines.credit_account_id "
                      "ORDER BY accounts.account_description;";

    // Simulate setting column widths and styles
    for (int i = 0; i < sizeof(colum_data) / sizeof(colum_data[0]); i++) {
        snprintf(cell, sizeof(cell), "%s%d", colum_data[i][0], line);
        // Here you would set the width, font, alignment, and fill for the cell
        printf("Setting column %s width to %s\n", colum_data[i][0], colum_data[i][2]);
        printf("Setting cell %s font and alignment\n", cell);
    }
}

int main() {
    create_excel();
    return 0;
}


void colorize_lines() {
    char cell_stack[256] = "";
    bool cell_flag = true;
    // Assuming PatternFill and other related structures are defined elsewhere
    PatternFill even = {wb_color, wb_color, "solid"};
    PatternFill odd = {"ffffff", "ffffff", "solid"};
    
    for (int row = 2; row < line; row++) {
        // Assuming get_row_value is a function that retrieves the value from the specified cell
        char* a = get_row_value(sheet, row, 2);
        
        if (strcmp(cell_stack, a) == 0) {
            // Do nothing
        } else {
            cell_flag = !cell_flag;
            strcpy(cell_stack, a);
        }
        
        PatternFill fill_pattern = cell_flag ? even : odd;
        
        for (int col = 1; col <= sheet.max_column; col++) {
            // Assuming set_cell_fill and set_cell_border are functions that set the fill and border of the cell
            set_cell_fill(sheet, row, col, fill_pattern);
            set_cell_border(sheet, row, col, dotted_border);
        }
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line = 0;
const int MAX_LINES = 1048576;
int SHEET_COUNT = -1;
int SHEET_NEW = 1; // True

// Function prototypes
void create_row_headers();
void create_sheet(const char* name);

void process_journals(char** journals, int journal_count) {
    // main loop
    for (int i = 0; i < journal_count; i++) { // lines_detail
        if (SHEET_NEW) {
            if (SHEET_COUNT == -1) {
                create_sheet("Diários");
            } else {
                char sheet_name[50];
                snprintf(sheet_name, sizeof(sheet_name), "Diários %d", SHEET_COUNT);
                create_sheet(sheet_name);
                // Assuming gl.MESSAGES_TO_EXCEL is a string buffer
                strcat(gl.MESSAGES_TO_EXCEL, "O numero de linhas excede o maximo suportado pelo Excel de 1 048 576, foi criada outra folha.\n");
            }
            SHEET_COUNT++;
            create_row_headers();
            SHEET_NEW = 0; // False
        }
    }
}

// Dummy implementations for the functions
void create_row_headers() {
    // Implementation for creating row headers
}

void create_sheet(const char* name) {
    // Implementation for creating a sheet
}


