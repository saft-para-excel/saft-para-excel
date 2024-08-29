#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *name;
    int width;
} ColumnData;

void create_excel() {
    // cria folha de artigos já com os saldos
    // Função para criar a célula do excel, a partir da coluna da linha
    char* cell(int col, int line) {
        static char cell_ref[5];
        return cell_ref;
    }
    printf("Creating sheet: Artigos\n");
    const char* wb_color = "e6e6ff";

    // -------- styles --------
    const char* title_background = "B4B6DC";
    const char* title_font = "Verdana, 8, bold, 000000";
    const char* money_format = "# ##0.00;[RED]-# ##0.00";
    const char* qtd_format = "# ##0.00;[RED]-# ##0.00";
    const char* center_align = "center, center";
    const char* dotted_border = "dotted";
    // ------------------------

    int line = 1;
    int col = 0;  // 0=A
    ColumnData colum_data[] = {
        {"Código", 32}, {"Nome", 50}, {"Familia", 20},
        {"Quantidade", 12}, {"Devoluções", 12},
        {"Stock", 12}, {"Credito", 12}, {"Debito", 12}, {"Saldo", 12},
        {"Credito c/", 12}, {"Débito c/", 12}, {"Saldo c/", 12},
        {"Tipo de Artigo", 12}
    };
    int num_columns = sizeof(colum_data) / sizeof(colum_data[0]);
    // cria as colunas
    for (col = 0; col < num_columns; col++) {
        printf("Setting width of column %s to %d\n", cell(col, 0), colum_data[col].width);
        printf("Setting font of cell %s to %s\n", cell(col, line), title_font);
        printf("Setting alignment of cell %s to %s\n", cell(col, line), center_align);
        printf("Setting fill of cell %s to %s\n", cell(col, line), title_background);
        printf("Setting value of cell %s to %s\n", cell(col, line), colum_data[col].name);
    }
    printf("Freezing panes at A2\n");  // primeira linha+1 fixa
}

