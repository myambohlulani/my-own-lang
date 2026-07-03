#ifndef PARSER_H
#define PARSER_H

#include "./lexer.hpp"
#include <optional>

// These nodes are for expressions and doing calculations	
// 0 + 2 , 2 - 3 ..etc
struct NodeExpr{
	Token int_lit;
};

// exit(0) ..etc
struct NodeExit {
	NodeExpr expr;
};

class Parser {
	/**
		Works similar to the Lexer,but goes through each and every token instead of char
	*/
	public:
		inline explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {
		}

		inline std::optional<NodeExit> parse() {
			std::optional<NodeExit> exit_node;

			while(peek().has_value()) {
				if(peek().value().type == TokenType::EXIT) {
					consume(); // consuming exit
					if(auto node_expr = parse_expr()) {
						exit_node = NodeExit{.expr = node_expr.value()};
					} else {
						std::cerr << "Invalid expression" << std::endl;
						exit(EXIT_FAILURE);
					}

					// semi colon
					if(peek().has_value() && peek().value().type == TokenType::SEMICOLON) {
						consume();
					} else {
						std::cerr << "There is no semicolon" << std::endl;
						exit(EXIT_FAILURE);
					} 

				}
			}
	
		
			m_index = 0; // reseting the value of the index to 0
			return exit_node;
		}
		/**
		This method parsers and expression
		*/
		inline std::optional<NodeExpr> parse_expr() {
			if(peek().has_value() && peek().value().type == TokenType::INT_LIT) {
				return NodeExpr{.int_lit = consume()}; // consuming the literal
			} else {
				return {};
			}	
		}

		// This method look for the next token with an offset of 0, can be incremented by an offset
		[[nodiscard]] inline std::optional<Token> peek(int offset = 1) const {
			if(m_index + offset > m_tokens.size()) {
				return {};
			} else {
				return m_tokens.at(m_index);
			}
		}

		/**
			This method consumes a token
		*/
		inline Token consume() {
			return m_tokens.at(m_index++);
		}

	private:
		const std::vector<Token> m_tokens;
		size_t m_index = 0;
};

#endif // PARSER_H
