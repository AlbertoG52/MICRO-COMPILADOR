#ifndef SYMBOL_H
#define SYMBOL_H

#define MAX_SYMBOLS 100
#define MAX_ID_LENGTH 32

typedef struct {
    char name[MAX_ID_LENGTH];
    int initialized; 
} SymbolEntry;

typedef struct {
    SymbolEntry symbols[MAX_SYMBOLS];
    int count;
} SymbolTable;

extern SymbolTable symbol_table;


void init_symbol_table(void);
int add_symbol(char *name);
int find_symbol(char *name);
void mark_initialized(char *name);
int is_initialized(char *name);
void check_all_initialized(void);

#endif
