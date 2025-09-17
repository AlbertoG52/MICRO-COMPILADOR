#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"

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

    // Inicializar el scanner (avanzar al primer carácter)
    Advance(src);

    // Probar el parser
    printf("Iniciando análisis sintáctico...\n");
    parse(src);  // Llama al parser recursivo descendente
    printf("Análisis sintáctico completado SIN ERRORES.\n");

    fclose(src);
    return 0;
}