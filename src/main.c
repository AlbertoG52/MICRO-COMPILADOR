#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parser.h"
#include "semantic.h"
#include "codegen.h"
#include "ast.h"

// Función para ejecutar comandos con verificación de errores
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

    // 1. ANÁLISIS SINTÁCTICO
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
    
    // 2. MOSTRAR AST PARA DEBUG
    printf("\n=== AST Generado ===\n");
    print_ast(ast, 0);
    printf("\n");

    // 3. ANÁLISIS SEMÁNTICO
    printf("=== Análisis Semántico ===\n");
    init_semantic_analysis();
    semantic_analysis(ast);
    printf("✓ Análisis semántico completado sin errores\n\n");

    // 4. GENERACIÓN DE CÓDIGO
    printf("=== Generación de Código ===\n");
    char asm_filename[256];
    snprintf(asm_filename, sizeof(asm_filename), "%s.asm", argv[1]);
    
    FILE *output = fopen(asm_filename, "w");
    if (!output) {
        perror("Error creando archivo assembly");
        free_ast(ast);
        return 1;
    }
    
    generate_program(ast, output);
    code_generation(ast, output);
    fclose(output);
    printf("✓ Archivo assembly generado: %s\n", asm_filename);

    // 5. VERIFICAR que el archivo .asm no esté vacío
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

    // 6. ENSAMBLAR Y LINKEAR CON VERIFICACIÓN
    printf("\n=== Ensamblado y Linkeo ===\n");
    char command[512];
    
    // Ensamblar
    snprintf(command, sizeof(command), "nasm -f elf64 %s.asm -o %s.o", argv[1], argv[1]);
    if (execute_command(command) != 0) {
        free_ast(ast);
        return 1;
    }
    
    // Linkear
    snprintf(command, sizeof(command), "gcc -no-pie %s.o -o %s", argv[1], argv[1]);
    if (execute_command(command) != 0) {
        free_ast(ast);
        return 1;
    }
    
    // Verificar que el ejecutable existe
    if (access(argv[1], F_OK) != 0) {
        fprintf(stderr, "ERROR: Ejecutable final no se creó\n");
        free_ast(ast);
        return 1;
    }
    
    printf("✓ Programa compilado exitosamente: %s\n", argv[1]);
    
    // 7. LIMPIAR (opcional)
    snprintf(command, sizeof(command), "rm %s.o", argv[1]);
    system(command); // Este puede fallar sin problema
    
    free_ast(ast);
    return 0;
}