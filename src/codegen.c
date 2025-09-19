#include "codegen.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

void code_generation(ASTNode *node, FILE *output) {
    if (!node){ 
        return;
    }

    switch (node->type){
    case NODE_PROGRAM:
        generate_program(node, output);
        
        if (node->left != NULL){
            code_generation(node->left, output); // Genera código del statement actual
        }
        break;

    case NODE_STMT_SEQUENCE:
            // ✅ Generar código para todos los statements en secuencia
            if (node->left != NULL) {
                code_generation(node->left, output);
            }
            if (node->right != NULL) {
                code_generation(node->right, output);
            }
            break;

    case NODE_ASSIGN:
        if (node->left == NULL || node->right == NULL)
        {
            fprintf(stderr, "ERROR: ASSIGN sin operandos\n");
            return;
        }
        code_generation(node->right, output);                      // Generar expresión
        fprintf(output, "    mov [%s], eax\n", node->left->value); // Guardar en variable
        break;

    case NODE_READ:
        if (node->left == NULL)
        {
            fprintf(stderr, "ERROR: READ sin variable\n");
            return;
        }
        fprintf(output, "    mov edi, format_int\n");
        fprintf(output, "    lea rsi, [%s]\n", node->left->value);
        fprintf(output, "    xor eax, eax\n");
        fprintf(output, "    call scanf\n");
        break;

    case NODE_WRITE:
        if (node->left == NULL)
        {
            fprintf(stderr, "ERROR: WRITE sin expresión\n");
            return;
        }
        code_generation(node->left, output);
        fprintf(output, "    mov edi, format_int\n");
        fprintf(output, "    mov esi, eax\n");
        fprintf(output, "    xor eax, eax\n");
        fprintf(output, "    call printf\n");
        break;

    case NODE_ADD:
        if (node->left == NULL || node->right == NULL)
        {
            fprintf(stderr, "ERROR: ADD sin operandos\n");
            return;
        }
        code_generation(node->left, output);
        fprintf(output, "    push rax\n");
        code_generation(node->right, output);
        fprintf(output, "    pop rbx\n");
        fprintf(output, "    add eax, ebx\n");
        break;

    case NODE_SUB:
        if (node->left == NULL || node->right == NULL)
        {
            fprintf(stderr, "ERROR: SUB sin operandos\n");
            return;
        }
        code_generation(node->left, output);
        fprintf(output, "    push rax\n");
        code_generation(node->right, output);
        fprintf(output, "    pop rbx\n");
        fprintf(output, "    sub eax, ebx\n");
        break;

    case NODE_VAR:
        if (node->value[0] == '\0')
        {
            fprintf(stderr, "ERROR: VAR sin nombre\n");
            return;
        }
        fprintf(output, "    mov eax, [%s]\n", node->value);
        break;

    case NODE_NUM:
        if (node->value[0] == '\0')
        {
            fprintf(stderr, "ERROR: NUM sin valor\n");
            return;
        }
        fprintf(output, "    mov eax, %s\n", node->value);
        break;

    default:
        fprintf(stderr, "ERROR: Tipo de nodo desconocido: %d\n", node->type);
        break;
    }
}

void generate_program(ASTNode *node, FILE *output) {
    // Encabezado assembly
    fprintf(output, "section .data\n");
    fprintf(output, "    format_int db '%%d', 0\n\n");
    
    fprintf(output, "section .bss\n");
    // Variables para test1.micro
    fprintf(output, "    x resd 1\n");
    fprintf(output, "    y resd 1\n");
    fprintf(output, "    z resd 1\n\n");
    
    fprintf(output, "section .text\n");
    fprintf(output, "    global main\n");
    fprintf(output, "    extern scanf, printf\n\n");
    fprintf(output, "main:\n");
}