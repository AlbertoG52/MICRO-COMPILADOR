#include "parser.h"
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Token currentToken;
static FILE *source;

Token getNextToken(FILE *src) {
    return Scanner(src);
}

int peek(TokenType expected_type) {
    return currentToken.type == expected_type;
}

int match(TokenType expected_type) {
    if (peek(expected_type)) {
        currentToken = getNextToken(source);
        return 1;
    }
    /*Este else por si hay un error pero es un despiche innecesario asi que lo dejo comentado*/
    /*else {
        char error_msg[100];
        snprintf(error_msg, 100, "Expected %s but got %s", 
                 token_type_to_str(expected_type), 
                 token_type_to_str(currentToken.type));
        error(error_msg);
        return 0;
    }*/
}

void error(const char *msg) {
    fprintf(stderr, "Parser error: %s\n", msg);
    exit(EXIT_FAILURE);
}

ASTNode* parse(FILE *src) {
    source = src;
    currentToken = getNextToken(source);
    ASTNode *ast = program();
    
    if (currentToken.type != T_EOF) {
        error("Expected EOF");
    }
    
    return ast;
}

ASTNode* program() {  
    printf("DEBUG: Entrando a program()\n");
    if (!match(T_BEGIN)) {  
        error("Expected 'begin'");
    }
    
    ASTNode *statements = stmts();  
    printf("DEBUG: statements = %p\n", (void*)statements);
    
    if (!match(T_END)) {  
        error("Expected 'end'");
    }
    
    ASTNode *program_node = create_node(NODE_PROGRAM);
    program_node->left = statements;
    printf("DEBUG: program_node creado en %p\n", (void*)program_node);
    return program_node;
}

ASTNode* stmts() {
    // Si no hay más statements (llegamos a END o EOF)
    if (currentToken.type == T_END || currentToken.type == T_EOF) {
        // Crear un nodo de secuencia vacío en lugar de NULL
        ASTNode *empty = create_node(NODE_STMT_SEQUENCE);
        return empty;
    }
    
    ASTNode *first_stmt = stmt();
    ASTNode *rest_stmts = stmts();
    
    // Si el resto de statements está vacío, devolver solo el primero
    if (rest_stmts != NULL && rest_stmts->type == NODE_STMT_SEQUENCE && 
        rest_stmts->left == NULL && rest_stmts->right == NULL) {
        free(rest_stmts); // Liberar el nodo vacío
        return first_stmt;
    }
    
    if (rest_stmts == NULL) {
        return first_stmt; // Solo hay un statement
    }
    
    // Crear nodo de secuencia
    ASTNode *seq_node = create_node(NODE_STMT_SEQUENCE);
    seq_node->left = first_stmt;
    seq_node->right = rest_stmts;
    return seq_node;
}

ASTNode* stmt() {
    if (peek(T_ID)) {
        // Asignación: ID = Expr;
        char id[33];
        strcpy(id, currentToken.lexeme);
        match(T_ID);
        match(T_ASSIGN);
        ASTNode *expr_node = expr();
        match(T_SEMICOLON);
        return create_assign_node(id, expr_node);
    } 
    else if (peek(T_READ)) {
        // read ID;
        match(T_READ);
        char id[33];
        strcpy(id, currentToken.lexeme);
        match(T_ID);
        match(T_SEMICOLON);
        return create_read_node(id);
    }
    else if (peek(T_WRITE)) {
        // write Expr;
        match(T_WRITE);
        ASTNode *expr_node = expr();
        match(T_SEMICOLON);
        return create_write_node(expr_node);
    }
    else {
        error("Expected statement (assignment, read, or write)");
        return NULL;
    }
}

ASTNode* expr() {
    ASTNode *left_term = term();
    return expr_prime(left_term);
}

ASTNode* expr_prime(ASTNode *left) {
    if (peek(T_PLUS)) {
        match(T_PLUS);
        ASTNode *right_term = term();
        ASTNode *add_node = create_bin_op_node(NODE_ADD, left, right_term);
        return expr_prime(add_node);
    }
    else if (peek(T_MINUS)) {
        match(T_MINUS);
        ASTNode *right_term = term();
        ASTNode *sub_node = create_bin_op_node(NODE_SUB, left, right_term);
        return expr_prime(sub_node);
    }
    return left; // λ production - return what we have
}

ASTNode* term() {
    if (peek(T_ID)) {
        char id[33];
        strcpy(id, currentToken.lexeme);
        match(T_ID);
        return create_var_node(id);
    }
    else if (peek(T_NUM)) {
        char num[33];
        strcpy(num, currentToken.lexeme);
        match(T_NUM);
        return create_num_node(num);
    }
    else {
        error("Expected identifier or number");
        return NULL;
    }
}