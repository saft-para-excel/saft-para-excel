#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uuid/uuid.h>

#include "accounts.h"
#include "header.h"
#include "journals.h"
#include "products.h"
#include "payments.h"
#include "sales.h"
#include "sqlserver_init.h"
#include "sqlserver_crud.h"
#include "stdio.h"
#include "crud.h"
#include "log_crud.h"
#include "stock_mov.h"
#include "suppliers.h"
#include "work_documents.h"

int read_file(char* file_name) {
    int linting = saft_lint_file(file_name);
    if (linting > 0) {  // failed linting
        log_crud_write_error("READ FILE ERROR: error on linting file: %d", linting);
        return 0;
    }
    FILE* f;
    if ((f = fopen(file_name, "r")) == NULL) {
        printf("Not a txt/unicode utf-8 file :\n");
        stdio_write_error_to_file();
        log_crud_write_error("READ FILE ERROR: not utf-8");
        return 0;
    }
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        gl_SAFT_FILE_DATA[gl_SAFT_FILE_DATA_len++] = strdup(line);
    }
    fclose(f);
    return 1;
}

int get_header(int* file_pointers, char* file_name) {
    header_dict_t header_dict;
    memset(&header_dict, 0, sizeof(header_dict));
    char h[37];
    stdio_calculate_hash(file_name, h);
    header_t* chk = sqlserver_crud_query_one("select file_name from header where file_hash= ?", h);
    if (chk == NULL) {
        strcpy(header_dict.file_hash, h);
        strcpy(header_dict.file_name, file_name);
    } else {
        return 0;  // same file, return false to continue
    }
    for (int p_line = file_pointers[0]; p_line < file_pointers[1]; p_line++) {
        tag_to_dict(gl_SAFT_FILE_DATA[p_line], "", &header_dict);
    }
    if (gl_GLOBAL_DICT.website[0] == '\0') {
        strcpy(gl_GLOBAL_DICT.website, "");
    }
    if (crud_insert_header(&header_dict)) {
    } else {
        return 0;
    }
    memcpy(&gl_GLOBAL_DICT, &header_dict, sizeof(header_dict_t));
    header_write_header_to_file(&header_dict);
    return 1;
}

void tag_to_dict(char* n, char* prefix, header_dict_t* header_dict) {
    int xsi = strstr(n, " xsi") - n;
    if (xsi >= 0) {
        int dum = strcspn(n, ">");
        memmove(n + xsi, n + dum + 1, strlen(n) - dum);
    }
    char line[1024];
    strcpy(line, n);
    char* a[3];
    int i = 0;
    char* p = strtok(line, "|");
    while (p != NULL) {
        a[i++] = p;
        p = strtok(NULL, "|");
    }
    if (i == 3) {
        char key[128];
        snprintf(key, sizeof(key), "%s%s", prefix, camel_to_snake(a[1]));
        strcpy(header_dict->data[header_dict->data_len++].key, key);
        strcpy(header_dict->data[header_dict->data_len++].value, a[2]);
    } else {
        strcpy(header_dict->data[header_dict->data_len++].key, "error");
        strcpy(header_dict->data[header_dict->data_len++].value, n);
    }
}

char* camel_to_snake(char* s) {
    static char buf[128];
    strcpy(buf, s);
    char* p = buf;
    while (*p) {
        if (isupper(*p)) {
            memmove(p + 1, p, strlen(p) + 1);
            *p = '_';
            p++;
        }
        p++;
    }
    if (buf[0] == '_') {
        memmove(buf, buf + 1, strlen(buf));
    }
    for (p = buf; *p; p++) {
        *p = tolower(*p);
    }
    return buf;
}

int saft_c_to_database(char* file_name) {
    int a[2];
    if (get_header(a, file_name)) {  // no error in header
        sqlserver_init_update_db_ledger();
        customers_get_customers(a);
        suppliers_get_suppliers(a);
        products_get_products(a);
        accounts_get_account(a);
        sales_get_sales_invoice(a);
        journals_insert_jornals(a);
        payments_get_payments(a);
        stock_mov_get_stock_movements(a);
        work_documents_get_work_documents(a);
    } else {
        return 0;
    }
    return 1;
}
void process_column_data(ColumnData *colum_data, int *cnt, Sheet *sheet, Font title_font, Alignment center_align, Fill title_background, GL gl) {
    for (int i = 0; i < colum_data->size; i++) {
        ColumnData cc = colum_data->data[i];
        char *col = gl.column_ref[*cnt];
        set_font(sheet, cell(), title_font);
        set_alignment(sheet, cell(), center_align);
        set_fill(sheet, cell(), title_background);
        set_value(sheet, cell(), cc);
        (*cnt)++;
        printf("%s,\n", cc);
    }
    sheet->freeze_panes = "A2";  // primeira linha+1 fixa
    const char *sql = "SELECT * FROM transactions ORDER BY transaction_id";
    Journals *journals = sqlite_crud_query_many(sql);
    line++;
    for (int j = 0; j < journals->size; j++) {  // lines_detail:
        int cnt = 0;
        for (int n = 0; n < journals->data[j].size; n++) {
            char *col = gl.column_ref[cnt];
            set_value(sheet, cell(), journals->data[j].data[n]);
            cnt++;
        }
        line++;
    }
}
#include <stdio.h>
#include <string.h>
#include <sqlite3.h>

void insert_journals(int file_pointers[2], char *SAFT_FILE_DATA[], sqlite3 *db) {
    int pointer_line = file_pointers[0];
    while (pointer_line <= file_pointers[1]) {
        char *n = SAFT_FILE_DATA[pointer_line];
        int flag = 0;
        if (strstr(n, "<Journal>") != NULL) {
            int journal_start = pointer_line;
            pointer_line += 1;
            char *journal_id = tag_to_string(SAFT_FILE_DATA[pointer_line]);
            pointer_line += 1;
            char *desc = tag_to_string(SAFT_FILE_DATA[pointer_line]);
            const char *sql = "INSERT INTO journals (journal_id, description) VALUES (?, ?)";
            printf("DEBUG %s\n", __FILE__);
            printf("journal_id, description, journal_hash\n");
            printf("%s, %s\n", journal_id, desc);
            printf("END DEBUG\n");

            sqlite3_stmt *stmt;
            sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            sqlite3_bind_text(stmt, 1, journal_id, -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, desc, -1, SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        if (strstr(n, "</Journal>") != NULL) {
            int journal_end = pointer_line;
            transaction_insert(journal_start, journal_end, journal_id);
        }
        pointer_line += 1;
    }
}




