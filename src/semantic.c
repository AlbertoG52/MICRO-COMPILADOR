#include "semantic.h"
#include "symbol.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void semantic_analysis(ASTNode *node) {
    if (node == NULL) return;
    
    switch (node->type) {
        case NODE_ASSIGN:
            // Asignación: x = expr;
            if (add_symbol(node->left->value)) {
                printf("✓ Variable '%s' declarada implícitamente\n", node->left->value);
            }
            mark_initialized(node->left->value);
            semantic_analysis(node->right); // Analizar expresión
            break;
            
        case NODE_READ:
            // read x;
            if (add_symbol(node->left->value)) {
                printf("✓ Variable '%s' declarada por read\n", node->left->value);
            }
            mark_initialized(node->left->value);
            break;
            
        case NODE_WRITE:
            // write expr;
            semantic_analysis(node->left);
            break;
            
        case NODE_VAR:
            // Uso de variable: x
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
            
        case NODE_ADD:
        case NODE_SUB:
            // Operaciones binarias
            semantic_analysis(node->left);
            semantic_analysis(node->right);
            break;
            
        case NODE_NUM:
            // Número literal - siempre válido
            break;
            
        default:
            // Recorrer hijos para otros nodos
            semantic_analysis(node->left);
            semantic_analysis(node->right);
    }
}

void init_semantic_analysis(void) {
    init_symbol_table();
}