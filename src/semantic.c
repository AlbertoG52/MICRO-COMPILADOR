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
            semantic_analysis(node->left);
            break;
            
        case NODE_STMT_SEQUENCE:
            semantic_analysis(node->left);
            semantic_analysis(node->right);
            break;
            
        case NODE_ASSIGN:
            semantic_analysis(node->right);
            
            if (find_symbol(node->left->value) == -1) {
                add_symbol(node->left->value);
                printf("✓ Variable '%s' declarada implícitamente por asignación\n", node->left->value);
            }
            
            mark_initialized(node->left->value);
            printf("✓ Asignación válida a variable '%s'\n", node->left->value);
            break;
            
        case NODE_READ:
            if (find_symbol(node->left->value) == -1) {
                add_symbol(node->left->value);
                printf("✓ Variable '%s' declarada por read\n", node->left->value);
            }
            
            mark_initialized(node->left->value);
            break;
            
        case NODE_WRITE:
            semantic_analysis(node->left);
            
            if (node->left->type == NODE_VAR && !is_initialized(node->left->value)) {
                fprintf(stderr, "Error semántico: Variable '%s' no inicializada\n", node->left->value);
                exit(EXIT_FAILURE);
            }
            printf("✓ Write válido\n");
            break;
            
        case NODE_ADD:
        case NODE_SUB:
            semantic_analysis(node->left);
            semantic_analysis(node->right);
            break;

        case NODE_VAR:
            if (find_symbol(node->value) == -1){
                add_symbol(node->value);
                printf("✓ Variable '%s' declarada implícitamente por uso\n", node->value);
            }
            break;

        case NODE_NUM:
            break;
            
        default:
            semantic_analysis(node->left);
            semantic_analysis(node->right);
    }
}

void init_semantic_analysis(void) {
    init_symbol_table();
}