#include "parser.h"
#include "semantic.h"
#include "codegen.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


void register_variable(char *name);
void generate_program_header(FILE *output);


void register_variables_from_ast(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_ASSIGN:
            register_variable(node->left->value);
            register_variables_from_ast(node->right);
            break;
        case NODE_READ:
            register_variable(node->left->value);
            break;
        case NODE_VAR:
            register_variable(node->value);
            break;
        default:
            register_variables_from_ast(node->left);
            register_variables_from_ast(node->right);
            break;
    }
}


int execute_command(const char *command) {
    printf("Ejecutando: %s\n", command);
    int result = system(command);
    if (result != 0) {
        fprintf(stderr, "ERROR: Comando falló con código %d\n", result);
        fprintf(stderr, "Comando: %s\n", command);
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo.micro>\n", argv[0]);
        return 1;
    }

    
    char base_name[256];
    strncpy(base_name, argv[1], sizeof(base_name) - 1);
    base_name[sizeof(base_name) - 1] = '\0';
    
    
    char *dot = strrchr(base_name, '.');
    if (dot) *dot = '\0';

   
    printf("=== Análisis Sintáctico ===\n");
    FILE *source = fopen(argv[1], "r");
    if (!source) {
        perror("Error abriendo archivo fuente");
        return 1;
    }
    
    ASTNode *ast = parse(source);
    fclose(source);
    
    if (ast == NULL) {
        fprintf(stderr, "ERROR: El AST es NULL después del parsing\n");
        return 1;
    }
    
   
    printf("\n=== AST Generado ===\n");
    print_ast(ast, 0);
    printf("\n");

    
    printf("=== Análisis Semántico ===\n");
    init_semantic_analysis();
    semantic_analysis(ast);
    printf("✓ Análisis semántico completado sin errores\n\n");

   
    printf("=== Generación de Código ===\n");
    char asm_filename[256];
    snprintf(asm_filename, sizeof(asm_filename), "%s.asm", base_name);
    
    FILE *output = fopen(asm_filename, "w");
    if (!output) {
        perror("Error creando archivo assembly");
        free_ast(ast);
        return 1;
    }
    
    
    register_variables_from_ast(ast);
    
    
    generate_program_header(output);
    
    
    if (ast != NULL && ast->left != NULL) {
        code_generation(ast->left, output);
    }
    
    
    fprintf(output, "\n    pop rbp         ; Restaurar stack\n");
    fprintf(output, "    mov eax, 0\n");
    fprintf(output, "    ret\n");

    fclose(output);
    printf("✓ Archivo assembly generado: %s\n", asm_filename);

    
    if (access(asm_filename, F_OK) != 0) {
        fprintf(stderr, "ERROR: Archivo .asm no existe\n");
        free_ast(ast);
        return 1;
    }
    
    FILE *check_asm = fopen(asm_filename, "r");
    fseek(check_asm, 0, SEEK_END);
    long file_size = ftell(check_asm);
    fclose(check_asm);
    
    if (file_size == 0) {
        fprintf(stderr, "ERROR: Archivo .asm está vacío\n");
        free_ast(ast);
        return 1;
    }
    
    printf("✓ Tamaño del archivo .asm: %ld bytes\n", file_size);

    
    printf("\n=== Ensamblado y Linkeo ===\n");
    char command[512];
    
   
    snprintf(command, sizeof(command), "nasm -f elf64 %s.asm -o %s.o", base_name, base_name);
    if (execute_command(command) != 0) {
        free_ast(ast);
        return 1;
    }
    
    
    snprintf(command, sizeof(command), "gcc -no-pie %s.o -o %s", base_name, base_name);
    if (execute_command(command) != 0) {
        free_ast(ast);
        return 1;
    }
    
    
    if (access(base_name, F_OK) != 0) {
        fprintf(stderr, "ERROR: Ejecutable final no se creó\n");
        free_ast(ast);
        return 1;
    }
    
    printf("✓ Programa compilado exitosamente: %s\n", base_name);
    
    
    snprintf(command, sizeof(command), "rm %s.o", base_name);
    int cleanup_result = system(command);
    (void)cleanup_result; 
    
    free_ast(ast);
    return 0;
}