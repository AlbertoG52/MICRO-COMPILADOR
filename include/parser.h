#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include "ast.h"
#include <stdio.h>

// Función principal del parser que devuelve AST
ASTNode* parse(FILE *src);

// Funciones para cada no terminal (ahora devuelven ASTNode*)
ASTNode* program();
ASTNode* stmts();
ASTNode* stmt();
ASTNode* expr();
ASTNode* expr_prime();
ASTNode* term();

// Funciones de ayuda para el parsing
int peek(TokenType expected_type);
int match(TokenType expected_type);
void error(const char *msg);

#endif
