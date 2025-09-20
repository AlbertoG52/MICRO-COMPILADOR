#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode* create_node(NodeType type) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    node->value[0] = '\0';
    return node;
}

ASTNode* create_assign_node(char *id, ASTNode *expr) {
    ASTNode *node = create_node(NODE_ASSIGN);
    node->left = create_var_node(id); // left child es la variable
    node->right = expr;               // right child es la expresión
    return node;
}

ASTNode* create_read_node(char *id) {
    ASTNode *node = create_node(NODE_READ);
    node->left = create_var_node(id); // left child es la variable a leer
    return node;
}

ASTNode* create_write_node(ASTNode *expr) {
    ASTNode *node = create_node(NODE_WRITE);
    node->left = expr; // left child es la expresión a escribir
    return node;
}

ASTNode* create_bin_op_node(NodeType op, ASTNode *left, ASTNode *right) {
    ASTNode *node = create_node(op);
    node->left = left;
    node->right = right;
    return node;
}

ASTNode* create_var_node(char *id) {
    ASTNode *node = create_node(NODE_VAR);
    strncpy(node->value, id, 32);
    node->value[32] = '\0';
    return node;
}

ASTNode* create_num_node(char *num) {
    ASTNode *node = create_node(NODE_NUM);
    strncpy(node->value, num, 32);
    node->value[32] = '\0';
    return node;
}

void print_ast(ASTNode *node, int depth) {
    if (node == NULL) {
        // Sangría para mostrar la profundidad
        for (int i = 0; i < depth; i++) printf("  ");
        printf("NULL\n");
        return;
    }
    
    // Sangría para mostrar la profundidad
    for (int i = 0; i < depth; i++) printf("  ");
    
    switch (node->type) {
        case NODE_STMT_SEQUENCE:
            printf("STMT_SEQUENCE\n");
            break;
        case NODE_PROGRAM:
            printf("PROGRAM\n");
            break;
        case NODE_ASSIGN:
            printf("ASSIGN\n");
            break;
        case NODE_READ:
            printf("READ\n");
            break;
        case NODE_WRITE:
            printf("WRITE\n");
            break;
        case NODE_ADD:
            printf("ADD\n");
            break;
        case NODE_SUB:
            printf("SUB\n");
            break;
        case NODE_VAR:
            printf("VAR: %s\n", node->value);
            return; // No tiene hijos
        case NODE_NUM:
            printf("NUM: %s\n", node->value);
            return; // No tiene hijos
        default:
            printf("UNKNOWN_NODE(%d)\n", node->type);
            return;
    }
    
    // Solo imprimir hijos si existen
    if (node->left != NULL || node->right != NULL) {
        print_ast(node->left, depth + 1);
        print_ast(node->right, depth + 1);
    }
}

void free_ast(ASTNode *node) {
    if (node == NULL) return;
    free_ast(node->left);
    free_ast(node->right);
    free(node);
}