#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"
#include "ast.h"
#include "semantic.h"

/*Como correr el main en la terminal
  1. make clean
  2. make
  3. ./bin/my_program <archivo.micro>, ejemplo: ./bin/my_program test.micro
*/


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo.micro>\n", argv[0]);
        return 1;
    }

    FILE *src = fopen(argv[1], "r");
    if (!src) {
        perror("Error opening file");
        return 1;
    }

    // Inicializar el scanner
    Advance(src);

    printf("🔨 Construyendo AST...\n");
    ASTNode *ast = parse(src);
    
    printf("✅ Análisis sintáctico completado\n");
    printf("\n🌳 AST:\n");
    print_ast(ast, 0);
    
    // ANÁLISIS SEMÁNTICO
    printf("\n🔍 Realizando análisis semántico...\n");
    init_semantic_analysis();
    semantic_analysis(ast);
    check_all_initialized();
    printf("✅ Análisis semántico completado\n");
    
    // Generación de código vendría aquí
    
    free_ast(ast);
    fclose(src);
    return 0;
}