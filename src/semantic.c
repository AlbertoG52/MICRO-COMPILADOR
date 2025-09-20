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
            semantic_analysis(node->left);
            break;
            
        case NODE_STMT_SEQUENCE:
            // Analizar statements en secuencia
            semantic_analysis(node->left);
            semantic_analysis(node->right);
            break;
            
        case NODE_ASSIGN:
            // ASIGNACIÓN: x = expr;
            // 1. Primero analizar la expresión (puede contener variables no declaradas)
            semantic_analysis(node->right);
            
            // 2. ✅ NUEVO: Si la variable no existe, declararla implícitamente
            if (find_symbol(node->left->value) == -1) {
                add_sysmbol(node->left->value);
                printf("✓ Variable '%s' declarada implícitamente por asignación\n", node->left->value);
            }
            
            // 3. Marcar como inicializada
            mark_initialized(node->left->value);
            printf("✓ Asignación válida a variable '%s'\n", node->left->value);
            break;
            
        case NODE_READ:
            // READ: read x;
            // 1. ✅ Si la variable no existe, declararla
            if (find_symbol(node->left->value) == -1) {
                add_symbol(node->left->value);
                printf("✓ Variable '%s' declarada por read\n", node->left->value);
            }
            
            // 2. Marcar como inicializada (read le da valor)
            mark_initialized(node->left->value);
            break;
            
        case NODE_WRITE:
            // WRITE: write expr;
            semantic_analysis(node->left);
            
            // ✅ Verificar que la expresión esté inicializada si es variable
            if (node->left->type == NODE_VAR && !is_initialized(node->left->value)) {
                fprintf(stderr, "Error semántico: Variable '%s' no inicializada\n", node->left->value);
                exit(EXIT_FAILURE);
            }
            printf("✓ Write válido\n");
            break;
            
        case NODE_ADD:
        case NODE_SUB:
            // Operaciones binarias
            semantic_analysis(node->left);
            semantic_analysis(node->right);
            break;

        case NODE_VAR:
            // USO de variable en expresión
            if (find_symbol(node->value) == -1){
                // Declarar implícitamente pero NO marcar como inicializada
                add_symbol(node->value);
                printf("✓ Variable '%s' declarada implícitamente por uso\n", node->value);
                // NO llamar a mark_initialized aquí - se marcará cuando se asigne
            }
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