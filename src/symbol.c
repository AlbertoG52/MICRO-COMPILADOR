#include "symbol.h"
#include <string.h>
#include <stdio.h>

// Variable global de la tabla de símbolos
SymbolTable symbol_table;

void init_symbol_table(void) {
    symbol_table.count = 0;
}

int add_symbol(char *name) {
    // Verificar si ya existe
    for (int i = 0; i < symbol_table.count; i++) {
        if (strcmp(symbol_table.symbols[i].name, name) == 0) {
            return 0; // Ya existe
        }
    }
    
    // Agregar nueva variable
    if (symbol_table.count < MAX_SYMBOLS) {
        strncpy(symbol_table.symbols[symbol_table.count].name, name, MAX_ID_LENGTH);
        symbol_table.symbols[symbol_table.count].initialized = 0;
        symbol_table.count++;
        return 1;
    }
    
    fprintf(stderr, "Error: Tabla de símbolos llena\n");
    return 0;
}

int find_symbol(char *name) {
    for (int i = 0; i < symbol_table.count; i++) {
        if (strcmp(symbol_table.symbols[i].name, name) == 0) {
            return i;
        }
    }
    return -1; // No encontrado
}

void mark_initialized(char *name) {
    int index = find_symbol(name);
    if (index != -1) {
        symbol_table.symbols[index].initialized = 1;
    }
}

int is_initialized(char *name) {
    int index = find_symbol(name);
    return (index != -1) ? symbol_table.symbols[index].initialized : 0;
}

void check_all_initialized(void) {
    for (int i = 0; i < symbol_table.count; i++) {
        if (!symbol_table.symbols[i].initialized) {
            fprintf(stderr, "Warning: Variable '%s' declarada pero no inicializada\n", 
                    symbol_table.symbols[i].name);
        }
    }
}