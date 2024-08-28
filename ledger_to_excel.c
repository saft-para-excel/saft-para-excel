#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

