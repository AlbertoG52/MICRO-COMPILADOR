#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "tokens.h"


Token Scanner(FILE *src);          
void Advance(FILE *src);         
int Peek(FILE *src);             
void SkipBlanks(FILE *src);       
Token ScanDigits(FILE *src);       

#endif