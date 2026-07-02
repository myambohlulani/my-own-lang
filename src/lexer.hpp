#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <optional>
#include <vector>

enum class TokenType {
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

	// functions
	RETURN,

	// display
	PRINT,

	// termination of the line
    SEMICOLON, 
	END_OF_LINE
};

typedef struct Token {
	TokenType type;
	std::optional<std::string> value {}; // can be Hlulani or any literal ..etc
} Token;

std::vector<Token> tokenize(const std::string& contents) {
	std::vector<Token> tokens{};
	std::string current_string{};
	
	for(int i = 0; i < contents.size(); i++) {
		char current_char = contents.at(i);

		if(std::isalpha(current_char)) {
			// starts with a char, hence either identifier or keyword
			while(std::isalnum(contents.at(i))) {
				current_string.push_back(contents.at(i)); // TODO: Check for single identifier value, i think it can throw errors
				i++;
			}

			i--;

			if (current_string == "int") { 
				tokens.push_back({.type = TokenType::INT_KEY});
			} else if (current_string == "return") {
				tokens.push_back({.type = TokenType::RETURN});
			} else {
				std::cout << "Default case" << std::endl;
			}

		} else if (std::isspace(current_char)) {
			continue; // ignoring space
		} else if(std::isdigit(current_char)) {
			current_string.push_back(current_char);
			i++;

			while(std::isdigit(contents.at(i))) {
				current_string.push_back(contents.at(i));
				i++;
			}

			i--;

			tokens.push_back({.type = TokenType::INT_LIT, .value = current_string});
		} else if (current_char == ';') {
			current_string.push_back(contents.at(i));
			tokens.push_back({.type = TokenType::SEMICOLON});
		} else {
			std::cerr << "You are at the end, maybe there is an error" << std::endl;
		}
		
			
		// debugging purpose
		std::cout << current_string << std::endl;
		// reseting the string to take the next token
		current_string.clear();
	}

	return tokens;
}

#endif // LEXER_H
