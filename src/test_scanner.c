#include <stdio.h>
#include "scanner.h"

int main() {
    FILE *src = fopen("test.micro", "r");
    if (!src) {
        perror("Error al abrir el archivo");
        return 1;
    }

    Token t;
    do {
        t = getNextToken(src);
        printf("Token: %d, Lexema: '%s'\n", t.type, t.lexeme);
    } while (t.type != T_EOF && t.type != T_ERROR);

    fclose(src);
    return 0;
}