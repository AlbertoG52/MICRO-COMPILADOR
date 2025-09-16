#include <stdio.h>
#include "scanner.h"

int main() {
    FILE *src = fopen("test.micro", "r");
    if (!src) {
        perror("Error opening file");
        return 1;
    }

    Advance(src); // Inicializar currentChar
    Token t;
    
    do {
        t = Scanner(src);
        printf("Token: %d, Lexeme: '%s'\n", t.type, t.lexeme);
    } while (t.type != T_EOF && t.type != T_ERROR);

    fclose(src);
    return 0;
}