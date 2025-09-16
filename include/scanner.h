#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "tokens.h"

// Estilo más parecido al libro
Token Scanner(FILE *src);          // Como getNextToken pero con nombre del libro
void Advance(FILE *src);           // Como fgetc pero con nombre significativo
int Peek(FILE *src);               // Como fpeek
void SkipBlanks(FILE *src);        // Como skipSpacesAndComments
Token ScanDigits(FILE *src);       // Como readNumber pero para números

#endif