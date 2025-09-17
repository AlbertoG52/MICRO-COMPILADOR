#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "tokens.h"
#include "scanner.h"

// Variable global para el carácter actual (estilo libro)
static int currentChar;

void Advance(FILE *src) {
    currentChar = fgetc(src);
}

int Peek(FILE *src) {
    int c = fgetc(src);
    ungetc(c, src);
    return c;
}

void SkipBlanks(FILE *src) {
    while (1) {
        if (currentChar == ' ' || currentChar == '\t' || currentChar == '\r') {
            Advance(src);
        } else if (currentChar == '\n') {
            Advance(src);
        } else if (currentChar == '-') {
            if (Peek(src) == '-') {
                // Es un comentario
                while (currentChar != '\n' && currentChar != EOF) {
                    Advance(src);
                }
            } else {
                break;
            }
        } else {
            break;
        }
    }
}

Token ScanDigits(FILE *src) {
    Token tok;
    int i = 0;
    
    while (isdigit(currentChar) && i < 32) {
        tok.lexeme[i++] = currentChar;
        Advance(src);
    }
    tok.lexeme[i] = '\0';
    tok.type = T_NUM;
    return tok;
}

Token Scanner(FILE *src) {
    Token ans;
    ans.line = 1; // Línea actual (simplificado)

    if(currentChar == 0) {
        Advance(src);
    }
    
    SkipBlanks(src);
    
    if (currentChar == EOF) {
        ans.type = T_EOF;
        strcpy(ans.lexeme, "");
        return ans;
    }
    
    if (isdigit(currentChar)) {
        return ScanDigits(src);
    }
    
    if (isalpha(currentChar)) {
        int i = 0;
        while ((isalnum(currentChar) || currentChar == '_') && i < 32) {
            ans.lexeme[i++] = currentChar;
            Advance(src);
        }
        ans.lexeme[i] = '\0';
        ans.type = checkReserved(ans.lexeme);
        return ans;
    }
    
    // Símbolos individuales
    switch (currentChar) {
        case '=':
            ans.type = T_ASSIGN;
            strcpy(ans.lexeme, "=");
            break;
        case '+':
            ans.type = T_PLUS;
            strcpy(ans.lexeme, "+");
            break;
        case '-':
            ans.type = T_MINUS;
            strcpy(ans.lexeme, "-");
            break;
        case ';':
            ans.type = T_SEMICOLON;
            strcpy(ans.lexeme, ";");
            break;
        default:
            ans.type = T_ERROR;
            sprintf(ans.lexeme, "Invalid char: '%c'", currentChar);
    }
    Advance(src);
    return ans;
}