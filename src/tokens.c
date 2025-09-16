#include "tokens.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Tabla de palabras reservadas
ReservedWord reserved_words[] = {
    {"begin", T_BEGIN},
    {"end",   T_END},
    {"read",  T_READ},
    {"write", T_WRITE},
    {NULL,    T_ERROR}
};

// Función para verificar si una cadena es palabra reservada
TokenType checkReserved(char *word) {
    for (int i = 0; reserved_words[i].word != NULL; i++) {
        if (strcmp(word, reserved_words[i].word) == 0) {
            return reserved_words[i].token;
        }
    }
    return T_ID; // No es reservada, es un identificador normal
}

// Función para mirar el próximo carácter sin consumirlo
int fpeek(FILE *src) {
    int c = fgetc(src);
    ungetc(c, src);
    return c;
}

// Salta espacios en blanco
void skipSpaces(FILE *src) {
    int c;
    while ((c = fgetc(src)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\r') {
            continue;
        } else {
            ungetc(c, src);
            break;
        }
    }
}

// Salta comentarios (-- hasta el fin de línea)
void skipComments(FILE *src) {
    int c;
    while ((c = fgetc(src)) != EOF) {
        if (c == '\n') {
            break;
        }
    }
}