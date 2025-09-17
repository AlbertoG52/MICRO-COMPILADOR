#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include <stdio.h>

// Función principal del parser
void parse(FILE *src);

// Funciones para cada no terminal
void program();
void stmts();
void stmt();
void expr();
void expr_prime();
void term();

// Manejo de errores
void error(const char *msg);

// Obtener el token actual
Token getNextToken(FILE *src);

#endif
