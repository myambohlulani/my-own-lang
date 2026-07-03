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
					output << "	mov rdi, " << tokens.at(i + 1).value.value() << "\n" << std::endl;
					output << "	syscall\n";
				}
			}
		}
	}

	return output.str();
}

class Lexer {
	public:
		// taking a copy of what i pass - string being passed
		inline explicit Lexer(const std::string& str) : m_str(std::move(str)) {
		}

		inline std::vector<Token> tokenize() {
			std::string current_string{};
			std::vector<Token> tokens{};

			while(look_next_character().has_value()) {
				char curr_char = look_next_character().value();

				if(std::isalpha(curr_char)) {
					current_string.push_back(pass_curr_char());

					while(look_next_character().has_value() && std::isalnum(look_next_character().value())) {
						current_string.push_back(pass_curr_char());
					}

					if (current_string == "int") {
						tokens.push_back({.type = TokenType::INT_KEY});
					} else if(current_string == "exit") {
						tokens.push_back({.type = TokenType::EXIT});
					} else {
						std::cerr << "Maybe an identifier?" << std::endl;
					}
                   
				//	std::cout << current_string << std::endl; // debugging

					current_string.clear(); // clearing the string

				} else if(std::isdigit(curr_char)) {
					current_string.push_back(pass_curr_char());

					while(look_next_character().has_value() && std::isdigit(look_next_character().value())) {
						current_string.push_back(pass_curr_char());
					}
	
					// debugging
					// std::cout << current_string << std::endl;

					tokens.push_back({.type = TokenType::INT_LIT, .value = current_string});
					current_string.clear();
				} else if(std::isspace(curr_char)) {
					pass_curr_char();
					continue;
				} else if (curr_char == ';') {
					tokens.push_back({.type = TokenType::SEMICOLON});
					pass_curr_char(); // consume
					// std::cout << "Semicolon" << std::endl; // for debugging
				} else {
					std::cerr << "Hahaha error" << std::endl; //error for debugging for now
					pass_curr_char(); // consume to avoid infinite loop
				}
			}

			m_curr_index = 0;
			return tokens;
		}	

	private:
		const std::string m_str;
		int m_curr_index = 0;
		

		[[nodiscard]] std::optional<char> look_next_character(int ahead = 0) const {
			/**
				This method peaks characters ahead, 1 is for default and you can specify the offset
				It does not change the contents of the class hence const and no-discard,
				This is same as peek in other compilers
			*/
			if(m_curr_index + ahead >= m_str.size()) {
				return {};
			} else {
				return m_str.at(m_curr_index + ahead);
			}
		}

		char pass_curr_char() {
			// This is similar to consume
			return m_str.at(m_curr_index++);
		}
};

#endif // LEXER_H
