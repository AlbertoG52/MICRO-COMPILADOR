#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"


void semantic_analysis(ASTNode *node);
void init_semantic_analysis(void);
void check_all_variables_initialized(void);

#endif