#ifndef TOKENS_H
#define TOKENS_H
#include <stdio.h>

typedef enum {

    T_BEGIN,
    T_END,
    T_READ,
    T_WRITE, 

    T_ID,
    T_NUM,


    T_ASSIGN,
    T_PLUS,
    T_MINUS,
    T_SEMICOLON,

    T_EOF,


    T_ERROR

} TokenType;

typedef struct {
    TokenType type;
    char lexeme[33];
    int line;
} Token;

// Tabla de palabras reservadas
typedef struct {
    char *word;
    TokenType token;
} ReservedWord;

// Declaración de la tabla de palabras reservadas
extern ReservedWord reserved_words[];

// Prototipos de funciones del scanner
Token getNextToken(FILE *src);
void skipComments(FILE *src);
void skipSpaces(FILE *src);
TokenType checkReserved(char *word);
int isReservedWord(char *word);
int fpeek(FILE *src);

#endif
