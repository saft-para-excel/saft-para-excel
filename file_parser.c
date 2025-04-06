#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char* get_value(const char* text, const char* limiter) {
    char* a = strstr(text, limiter);
    char* b = strrstr(text, limiter);
    if (a && b && a < b) {
        size_t length = b - (a + strlen(limiter));
        char* value = (char*)malloc(length + 1);
        strncpy(value, a + strlen(limiter), length);
        value[length] = '\0';
        return value;
    }
    return NULL;
}

char* get_encoding(const char* text) {
    char* a = strstr(text, "encoding");
    if (!a) {
        return "False";
    }
    char* b = strchr(a, '?');
    if (b) {
        return get_value(a, "\"");
    }
    return NULL;
}

bool is_multidocumento(const char* text) {
    return strstr(text, "eFaturaMD") != NULL;
}

bool is_e_fatura(const char* text) {
    return strstr(text, "Setor") != NULL || strstr(text, "Forncecedor") != NULL;
}

bool is_inventario(const char* text) {
    return strstr(text, "StockFile") != NULL;
}

int main() {
    FILE* file = fopen("/home/zorze/SAFT/files/inventario.xml", "rb");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char data[4097];
    fread(data, sizeof(char), 4096, file);
    data[4096] = '\0'; // Null-terminate the string

    fclose(file);

    printf("%s\n", data);
    printf("%s\n", get_encoding(data));
    printf("%s\n", is_multidocumento(data) ? "true" : "false");
    printf("%s\n", is_e_fatura(data) ? "true" : "false");
    printf("%s\n", is_inventario(data) ? "true" : "false");

    return EXIT_SUCCESS;
}
