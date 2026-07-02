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
	EXIT,

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
			} else if (current_string == "exit") {
				tokens.push_back({.type = TokenType::EXIT});
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
		//std::cout << current_string << std::endl;
		// reseting the string to take the next token
		current_string.clear();
	}

	return tokens;
}

std::string tokens_to_asm(const std::vector<Token> tokens) {
	std::stringstream output;

	// giving it values at the top
	output << "global _start\n_start:\n";
	
	for(int i = 0; i < tokens.size(); i++) {
		const Token& token = tokens.at(i);

		if(token.type == TokenType::EXIT) { // checking if i start with a return
			if(i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::INT_LIT) {
				if(i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::SEMICOLON) {
					output << "	mov rax, 60\n";
					output << "	mov rdi, " << tokens.at(i + 1).value.value() << std::endl;
					output << "	syscall";
				}
			}
		}
	}

	return output.str();
}

class Lexer {
	public:
		// taking a copy of what i pass - string being passed
		inline Lexer(std::string& str) : m_str(std::move(str)) {
		}

		inline std::vector<Token> tokenize() {

		}

	private:
		const std::string m_str;
		int m_curr_index = 0;

		[[nodiscard]] std::optional<char> look_next_character(int ahead = 1) const {
			/**
				This method peaks characters ahead, 1 is for default and you can specify the offset
				It does not change the contents of the class hence const and no-discard,
				This is same as peek in other compilers
			*/
			if(m_curr_index + ahead >= m_str.size() {
				return {};
			} else {
				return m_str.at(m_curr_index);
			}
		}

		char pass_curr_char() {
			// This is similar to consume
			return m_str.at(m_curr_index++);
		}
}

#endif // LEXER_H
