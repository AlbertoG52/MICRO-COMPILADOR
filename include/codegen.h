#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "ast.h"

void code_generation(ASTNode *node, FILE *output);
void generate_program(ASTNode *node, FILE *output);
void register_variable(char *name);

#endif