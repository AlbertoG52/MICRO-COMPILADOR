#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tokens.h"
#include "scanner.h"

// Función principal del scanner
Token getNextToken(FILE *src) {
    Token t;
    t.line = 1; // En una versión real, llevarías un contador de líneas

    // Saltar espacios y comentarios
    skipSpacesAndComments(src);

    if (feof(src)) {
        t.type = T_EOF;
        strcpy(t.lexeme, "");
        return t;
    }

    char c = fgetc(src);

    // Identificadores o palabras reservadas
    if (isalpha(c)) {
        ungetc(c, src);
        readIdentifier(src, t.lexeme);
        t.type = checkReserved(t.lexeme);
    }
    // Números
    else if (isdigit(c)) {
        ungetc(c, src);
        readNumber(src, t.lexeme);
        t.type = T_NUM;
    }
    // Símbolos especiales
    else {
        switch (c) {
            case '=':
                t.type = T_ASSIGN;
                strcpy(t.lexeme, "=");
                break;
            case '+':
                t.type = T_PLUS;
                strcpy(t.lexeme, "+");
                break;
            case '-':
                // Verificar si es comentario
                if (fpeek(src) == '-') {
                    skipComment(src);
                    return getNextToken(src); // Llamada recursiva
                } else {
                    t.type = T_MINUS;
                    strcpy(t.lexeme, "-");
                }
                break;
            case ';':
                t.type = T_SEMICOLON;
                strcpy(t.lexeme, ";");
                break;
            default:
                t.type = T_ERROR;
                sprintf(t.lexeme, "Carácter inválido: '%c'", c);
        }
    }
    return t;
}

// Lee un identificador (letras seguidas de letras/dígitos)
void readIdentifier(FILE *src, char *lexeme) {
    int i = 0;
    char c;
    while ((c = fgetc(src)) != EOF && (isalnum(c) || c == '_') && i < 32) {
        lexeme[i++] = c;
    }
    lexeme[i] = '\0';
    if (c != EOF) ungetc(c, src);
}

// Lee un número (dígitos)
void readNumber(FILE *src, char *lexeme) {
    int i = 0;
    char c;
    while ((c = fgetc(src)) != EOF && isdigit(c) && i < 32) {
        lexeme[i++] = c;
    }
    lexeme[i] = '\0';
    if (c != EOF) ungetc(c, src);
}

// Salta espacios y comentarios
void skipSpacesAndComments(FILE *src) {
    int c;
    while ((c = fgetc(src)) != EOF) {
        if (c == ' ' || c == '\t' || c == '\r') {
            continue;
        } else if (c == '\n') {
            // Incrementar contador de líneas aquí si lo llevas
            continue;
        } else if (c == '-') {
            if (fpeek(src) == '-') {
                skipComment(src);
            } else {
                ungetc(c, src);
                break;
            }
        } else {
            ungetc(c, src);
            break;
        }
    }
}

// Salta un comentario (-- hasta fin de línea)
void skipComment(FILE *src) {
    int c;
    while ((c = fgetc(src)) != EOF && c != '\n');
}