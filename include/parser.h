#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include "ast.h"
#include <stdio.h>


ASTNode* parse(FILE *src);


ASTNode* program();
ASTNode* stmts();
ASTNode* stmt();
ASTNode* expr();
ASTNode* expr_prime();
ASTNode* term();


int peek(TokenType expected_type);
int match(TokenType expected_type);
void error(const char *msg);

#endif
