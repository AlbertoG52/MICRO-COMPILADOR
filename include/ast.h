#ifndef AST_H
#define AST_H

typedef enum {
    NODE_PROGRAM,
    NODE_STMT_SEQUENCE,
    NODE_ASSIGN,
    NODE_READ,
    NODE_WRITE,
    NODE_ADD,
    NODE_SUB,
    NODE_VAR,
    NODE_NUM
} NodeType;

typedef struct ASTNode {
    NodeType type;
    struct ASTNode *left;
    struct ASTNode *right;
    char value[33]; 
} ASTNode;


ASTNode* create_node(NodeType type);
ASTNode* create_assign_node(char *id, ASTNode *expr);
ASTNode* create_read_node(char *id);
ASTNode* create_write_node(ASTNode *expr);
ASTNode* create_bin_op_node(NodeType op, ASTNode *left, ASTNode *right);
ASTNode* create_var_node(char *id);
ASTNode* create_num_node(char *num);


void print_ast(ASTNode *node, int depth);
void free_ast(ASTNode *node);

#endif