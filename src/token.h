
/**
* token.h
* Token interface
*/
#ifndef TOKEN_H
#define TOKEN_H

typedef char* string;
typedef enum {
    // Data types
    TOKEN_I32, TOKEN_I64, TOKEN_F32, TOKEN_F64, TOKEN_STRING, TOKEN_BOOL,
    // literals
    TOKEN_IDENTIFIER, TOKEN_NUMBER, TOKEN_STRING_LITERAL, TOKEN_TRUE, TOKEN_FALSE,
    // stdout
    TOKEN_PRINT, TOKEN_PRINTF,

    // operators
    TOKEN_ASSIGN, TOKEN_INCREMENT, TOKEN_DECREMENT
    // delimeters
    TOKEN_SEMI_COLON, TOKEN_OPEN_CURLY, TOKEN_CLOSE_CURLY, TOKEN_EOF

} TokenType;

typedef struct {
    TokenType type;
    string value;
    int line_number;
    int column;
}Token;

Token* create_token(TokenType token_type, const string value, int line, int column);
void free_token(Token* token);
void print_token(Token* token);
const string convert_token_to_string(Token_type type);

#endif // TOKEN_H