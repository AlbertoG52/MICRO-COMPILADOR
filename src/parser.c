#include "parser.h"
#include "tokens.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token currentToken;
static FILE *source;

Token getNextToken(FILE *src) {
    return Scanner(src);
}

void parse(FILE *src) {
    source = src;
    currentToken = getNextToken(source);
    program();
    if (currentToken.type != T_EOF) {
        error("Expected EOF");
    }
}

void program() {
    if (currentToken.type == T_BEGIN) {
        currentToken = getNextToken(source);
        stmts();
        if (currentToken.type == T_END) {
            currentToken = getNextToken(source);
        } else {
            error("Expected 'end'");
        }
    } else {
        error("Expected 'begin'");
    }
}

void stmts() {
    if (currentToken.type == T_ID || currentToken.type == T_READ || currentToken.type == T_WRITE) {
        stmt();
        stmts();
    }
    // λ: no hacer nada
}

void stmt() {
    if (currentToken.type == T_ID) {
        // Asignación: ID ASSIGN Expr SEMICOLON
        char id[33];
        strcpy(id, currentToken.lexeme);
        currentToken = getNextToken(source);
        
        if (currentToken.type == T_ASSIGN) {
            currentToken = getNextToken(source);
            expr();
            if (currentToken.type == T_SEMICOLON) {
                currentToken = getNextToken(source);
            } else {
                error("Expected ';'");
            }
        } else {
            error("Expected '='");
        }
    } else if (currentToken.type == T_READ) {
        // READ ID SEMICOLON
        currentToken = getNextToken(source);
        if (currentToken.type == T_ID) {
            currentToken = getNextToken(source);
            if (currentToken.type == T_SEMICOLON) {
                currentToken = getNextToken(source);
            } else {
                error("Expected ';' after read");
            }
        } else {
            error("Expected identifier after read");
        }
    } else if (currentToken.type == T_WRITE) {
        // WRITE Expr SEMICOLON
        currentToken = getNextToken(source);
        expr();
        if (currentToken.type == T_SEMICOLON) {
            currentToken = getNextToken(source);
        } else {
            error("Expected ';' after write");
        }
    } else {
        error("Expected statement");
    }
}

void expr() {
    term();
    expr_prime();
}

void expr_prime() {
    if (currentToken.type == T_PLUS || currentToken.type == T_MINUS) {
        TokenType op = currentToken.type;
        currentToken = getNextToken(source);
        term();
        expr_prime();
    }
    // λ: no hacer nada
}

void term() {
    if (currentToken.type == T_ID || currentToken.type == T_NUM) {
        currentToken = getNextToken(source);
    } else {
        error("Expected identifier or number");
    }
}

void error(const char *msg) {
    fprintf(stderr, "Parser error: %s\n", msg);
    exit(EXIT_FAILURE);
}
