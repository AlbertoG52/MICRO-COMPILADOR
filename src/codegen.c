#include "codegen.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

#include "codegen.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Tabla de variables dinámica
typedef struct {
    char name[33];
    int used;
} Variable;

static Variable variables[100];
static int var_count = 0;

// Registrar variable si no existe
void register_variable(char *name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].used = 1;
            return;
        }
    }
    
    if (var_count < 100) {
        strncpy(variables[var_count].name, name, 32);
        variables[var_count].used = 1;
        var_count++;
    }
}

// Generar encabezado con variables dinámicas
void generate_program_header(FILE *output) {
    fprintf(output, "section .data\n");
    fprintf(output, "    format_int db '%%d', 0\n\n");
    fprintf(output, "    format_int_newline db '%%d', 10, 0\n\n");
    
    fprintf(output, "section .bss\n");
    for (int i = 0; i < var_count; i++) {
        if (variables[i].used) {
            fprintf(output, "    %s resd 1\n", variables[i].name);
        }
    }
    fprintf(output, "\n");
    
    fprintf(output, "section .text\n");
    fprintf(output, "    global main\n");
    fprintf(output, "    extern scanf, printf\n\n");
    fprintf(output, "main:\n");
    fprintf(output, "    push rbp\n");
    fprintf(output, "    mov rbp, rsp\n\n");
}

void code_generation(ASTNode *node, FILE *output){
    if (!node)
    {
        return;
    }

    switch (node->type)
    {
    case NODE_PROGRAM:
        if (node->left != NULL)
        {
            code_generation(node->left, output);
        }
        break;

    case NODE_STMT_SEQUENCE:
        if (node->left != NULL)
        {
            code_generation(node->left, output);
        }
        if (node->right != NULL)
        {
            code_generation(node->right, output);
        }
        break;

    case NODE_ASSIGN:
        if (node->left == NULL || node->right == NULL){
            fprintf(stderr, "ERROR: ASSIGN sin operandos\n");
            return;
        }

        // ✅ Solo generar la expresión si NO es una constante inmediata después de read
        code_generation(node->right, output);
        fprintf(output, "    mov [%s], eax\n", node->left->value);
        break;

    case NODE_READ:
        if (node->left == NULL){
            fprintf(stderr, "ERROR: READ sin variable\n");
            return;
        }
        register_variable(node->left->value);
        fprintf(output, "    mov rdi, format_int\n");
        fprintf(output, "    lea rsi, [%s]\n", node->left->value);
        fprintf(output, "    xor eax, eax\n"); // ✅ AL=0 para scanf varargs
        fprintf(output, "    call scanf\n");
        break;

    case NODE_WRITE:
        if (node->left == NULL){
            fprintf(stderr, "ERROR: WRITE sin expresión\n");
            return;
        }
        code_generation(node->left, output);
        fprintf(output, "    mov rdi, format_int_newline\n\n");
        fprintf(output, "    mov esi, eax\n");
        fprintf(output, "    xor eax, eax\n"); // ✅ AL=0 para printf varargs
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
        // Registrar variable usada
        register_variable(node->value);
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




