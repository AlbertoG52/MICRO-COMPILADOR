#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.h"
#include "semantic.h"

void test_semantic(const char *filename, const char *description) {
    printf("\n=== TEST: %s ===\n", description);
    printf("Archivo: %s\n", filename);
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("❌ No se pudo abrir el archivo\n");
        return;
    }
    
    init_semantic_analysis();
    
    printf("--- Análisis Sintáctico ---\n");
    ASTNode *ast = parse(file);
    if (ast) {
        printf("✓ AST generado correctamente\n");
        printf("\n--- Árbol AST ---\n");
        print_ast(ast, 0);
        
        printf("\n--- Análisis Semántico ---\n");
        semantic_analysis(ast);
        check_all_variables_initialized();
        printf("✓ Análisis semántico completado\n");
        
        free_ast(ast);
    } else {
        printf("❌ Error en parsing\n");
    }
    
    fclose(file);
    printf("-----------------------------\n");
}

int main() {
    // Pruebas que deberían FUNCIONAR
    test_semantic("test1.micro", "Programa válido con read y asignación");
    test_semantic("test3.micro", "Solo operaciones con variables leídas");
    
    // Pruebas que deberían FALLAR
    test_semantic("test_error1.micro", "Variable no declarada en asignación");
    test_semantic("test_error2.micro", "Variable no inicializada en expresión");
    test_semantic("test_error3.micro", "Uso de variable sin read previo");
    
    return 0;
}