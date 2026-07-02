#ifndef LEXER_H
#define LEXER_H
#include <iostream>

enum TokenType {
	INDENTIFIER,
    // control flow
    IF_KEY,
    ELSEIF_KEY,
    ELSE_KEY,
    WHILE_KEY,
    FOR_KEY,

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

	// Symbols
	OP_PAREN,
	CL_PAREN,
	OP_CURLY,
	CL_CURLY,

	// Comments
	COMMENT,

	// literals
	INT_LIT,
	STRING_LIT,
	FLOAT_LIT,
	DOUBLE_LIT,

	// display
	PRINT,

	// termination of the line
    SEMICOLON, 
	END_OF_LINE
};

struct Token {
	TokenType type;
	std::string raw_data; // can be Hlulani or any literal ..etc
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
		int current_index = 0;

		char advance() {
			return contents[current_index++];
		}

		void find_token() {
			char current_char = advance();
			switch (current_char) {
				// data types
				case 'int': 
						push_token_inwards(TokenType::INT_KEY); 
					break;
				case 'float':
						push_token_inwards(TokenType::FLOAT_KEY);
					break;
				case 'double':
						push_token_inwards(TokenType::DOUBLE_KEY);
					break;
				case 'char*':
						push_token_inwards(TokenType::STRING_KEY);
					break;
				case ';':
						push_token_inwards(TokenType::SEMICOLON);
					break;
				case "{":
						push_token_inwards(TokenType::OP_CURLY);
					break;
				case "}":
						push_token_inwards(TokenType::CL_CURLY);
					break;
				case "(":
						push_token_inwards(TokenType::OP_PAREN);
					break;
				case ")":
						push_token_inwards(TokenType::CL_PAREN);
					break;
				case "printf":
						push_token_inwards(TokenType::PRINT);
					break;
			}
		}

		void push_token_inwards(TokenType type) {
			tokens.push_back({type, line});
		}
}

#endif // LEXER_H
