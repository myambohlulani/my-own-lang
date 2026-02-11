package com.framework;

public enum TokenType {
    LEFT_PAREN, RIGHT_PARENT, LEFT_BRACE, RIGHT_BRACE, SEMICOLON, ASTERISK,
    // literals
    IDENTIFIER, STRING, NUMBER,
    // types
    I32, I64, F32, F64, BOOL,
    // keywords
    AND, CLASS, ELSE, FALSE, FOR, IF, OR, PRINT, PRINTF, RETURN, TRUE, WHILE,

    EOF
};