
/**
* lexer.h
* Lexical Analyzer or tokenizer interface
* @author Hlulani myambo
* @date 5 Feb 2026
*/

#ifndef LEXER_H
#define LEXER_H

#include "token.h"

typedef char* string;

typedef struct Lexer {
    string source;
    int position;
    int line;
    int column;
    string current;
} Lexer;

Lexer* create_lexer(const string source);
void free_lexer(Lexer* lexer);
Token* next_lexer_token(Lexer* lexer);
Token** tokenize_lexer(Lexer* lexer, int* count);

#endif // LEXER_H