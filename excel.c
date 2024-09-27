#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <saft.h>

#define MAX_COLUMNS 23
#define MAX_COLUMN_NAME_LENGTH 40
#define MAX_COLUMN_WIDTH 40

typedef struct {
    char name[MAX_COLUMN_NAME_LENGTH];
    int width;
} ColumnData;

int main() {
    int line = 1;
    int col = 0;  // 0=A
    ColumnData colum_data[MAX_COLUMNS] = {
        {"Numero", 15}, {"Data", 15}, {"Cliente", 40}, {"Morada", 40}, {"Localidade", 15},
        {"Codigo Postal", 10}, {"NIF", 10}, {"Código", 30}, {"Artigo", 35}, {"Qtd.", 10},
        {"Und.", 5}, {"P. Un.", 10}, {"Total s/", 10}, {"IVA", 10}, {"Total c/", 10},
        {"Taxa IVA", 8}, {"Data/Hora", 20}, {"Cod. Isenção", 10}, {"Motivo Isenção", 35},
        {"AT-CUD", 10}, {"Descrição Suplementar", 20}, {"Doc. Origem", 30}, {"M. Pagamento", 20},
        {"Familia", 20}
    };

    // create header row
    for (int i = 0; i < MAX_COLUMNS; i++) {
        printf("sheet.column_dimensions[%c].width = %d;  // width\n", 'A' + col, colum_data[col].width);
        printf("sheet[\"%c%d\"].font = title_font;\n", 'A' + col, line);
        printf("sheet[\"%c%d\"].alignment = center_align;\n", 'A' + col, line);
        printf("sheet[\"%c%d\"].fill = title_background;\n", 'A' + col, line);
        printf("sheet[\"%c%d\"] = \"%s\";\n", 'A' + col, line, colum_data[col].name);
        col++;
    }

    // add comment to cell
    printf("Comment comment = Comment(\"Numero do Documento\", \"SAF-t para Excel\");\n");
    printf("sheet[\"A1\"].comment = comment;\n");

    return 0;
}

