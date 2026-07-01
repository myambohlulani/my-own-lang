#ifndef LEXER_H
#define LEXER_H
#include <iostream>

enum TokenType {
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

	// Data types
	INT_KEY,
	STRING_KEY,
	FLOAT_KEY,
	DOUBLE_KEY,

	// termination of the line
    SEMICOLON, 
	END_OF_LINE
};

struct Token {
	TokenType type;
	std::string contents;
	int line_number;
} Token;


class Lexer {
	public:
		Lexer(std::string contents): contents(contents) {
		}

		std::vector<Token> tokenize() {

		}
	
	private:
		std::string contents;
		std::vector<Token> tokens;

}

#endif // LEXER_H
