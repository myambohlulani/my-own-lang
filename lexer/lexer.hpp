#ifndef LEXER_H
#define LEXER_H

enum Token {
    // control flow
    IF_KEY,
    ELSEIF_KEY,
    ELSE_KEY,
    WHILE_KEY,
    FOR_KEY,
    // data type
    INT_KEY,

    // operators
    EQUALS_OP,
    PLUS_OP,
    DIVIDE_OP,
    MINUS_OP,

    SEMICOLON
};

#endif // LEXER_H