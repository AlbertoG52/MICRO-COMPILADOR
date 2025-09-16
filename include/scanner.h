#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "tokens.h"

Token getNextToken(FILE *src);
void readIdentifier(FILE *src, char *lexeme);
void readNumber(FILE *src, char *lexeme);
void skipSpacesAndComments(FILE *src);
void skipComment(FILE *src);

#endif