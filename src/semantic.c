#include "semantic.h"
#include "symbol.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void semantic_analysis(ASTNode *node) {
    if (node == NULL) return;
    
    switch (node->type) {
        case NODE_PROGRAM:
            // Analizar todos los statements del programa
            semantic_analysis(node->left); // Primer statement
            semantic_analysis(node->right); // Siguientes statements (si hay secuencia)
            break;
            
        case NODE_ASSIGN:
            // ASIGNACIÓN: x = expr;
            // 1. Verificar que la expresión es válida
            semantic_analysis(node->right);
            
            // 2. Verificar que la variable existe (debe haber sido declarada con read)
            if (find_symbol(node->left->value) == -1) {
                fprintf(stderr, "Error semántico: Variable '%s' no declarada (use read primero)\n", node->left->value);
                exit(EXIT_FAILURE);
            }
            
            // 3. Marcar como inicializada
            mark_initialized(node->left->value);
            printf("✓ Asignación válida a variable '%s'\n", node->left->value);
            break;
            
        case NODE_READ:
            // READ: read x;
            // 1. Agregar variable a la tabla (declaración implícita)
            if (add_symbol(node->left->value)) {
                printf("✓ Variable '%s' declarada por read\n", node->left->value);
            }
            
            // 2. Marcar como inicializada (read le da valor)
            mark_initialized(node->left->value);
            break;
            
        case NODE_WRITE:
            // WRITE: write expr;
            semantic_analysis(node->left);
            printf("✓ Write válido\n");
            break;
            
        case NODE_ADD:
        case NODE_SUB:
            // Operaciones binarias: expr + expr
            semantic_analysis(node->left);
            semantic_analysis(node->right);
            break;
            
        case NODE_VAR:
            // USO de variable: x (en expresión)
            if (find_symbol(node->value) == -1) {
                fprintf(stderr, "Error semántico: Variable '%s' no declarada\n", node->value);
                exit(EXIT_FAILURE);
            }
            if (!is_initialized(node->value)) {
                fprintf(stderr, "Error semántico: Variable '%s' no inicializada\n", node->value);
                exit(EXIT_FAILURE);
            }
            printf("✓ Uso válido de variable '%s'\n", node->value);
            break;
            
        case NODE_NUM:
            // Número literal - siempre válido
            break;
            
        default:
            // Para cualquier otro tipo de nodo, recorrer hijos
            semantic_analysis(node->left);
            semantic_analysis(node->right);
    }
}

void init_semantic_analysis(void) {
    init_symbol_table();
}

void check_all_variables_initialized(void) {
    // Esta función verifica al final del análisis si hay variables no inicializadas
    // (ejercicio 10 - aunque read siempre inicializa, por si acaso)
    for (int i = 0; i < symbol_table.count; i++) {
        if (!symbol_table.symbols[i].initialized) {
            fprintf(stderr, "Warning: Variable '%s' declarada pero no inicializada\n", 
                    symbol_table.symbols[i].name);
        }
    }
}