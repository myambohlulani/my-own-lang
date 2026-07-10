#ifndef PARSER_H
#define PARSER_H

#include "./lexer.hpp"
#include <optional>

struct NodeExpr {
  Token int_lit;
};

struct NodeExit {
  NodeExpr expr;
};

struct NodeIdent {
  NodeExpr expr;
  Token name;
};

struct NodeStr {
  Token string_lit;
};

class Parser {
  /**
          Works similar to the Lexer,but goes through each and every token
     instead of char
  */
public:
  inline explicit Parser(const std::vector<Token> &tokens) : m_tokens(tokens) {}

  inline std::optional<NodeExit> parse_exit() {
    std::optional<NodeExit> exit_node {};

    while (peek().has_value()) {
      if (peek().value().type == TokenType::EXIT && peek(1).has_value() && peek(1).value().type == TokenType::OP_PAREN) {
        consume(); // consume exit
        consume(); // consume the next open parentheses

        // TODO: check for the value
        if (const auto node_expr = parse_expr()) {
          exit_node  = NodeExit {.expr = node_expr.value()};
        }

        // TODO: Check close paren
        if (peek().has_value() && peek().value().type == TokenType::CL_PAREN) {
          consume();
        } else {
          std::cout << "There is no close parentheses in the exit" << std::endl;
        }

          // TODO: Check semi-colon
        if (peek().has_value() && peek().value().type == TokenType::SEMICOLON) {
          consume();
        } else {
          std::cout << "There is no semicolon in the exit" << std::endl;
          exit(EXIT_FAILURE);
        }
      }
    }

    return exit_node;
  }

  /**
     This method parsers and expression
   */
  inline std::optional<NodeExpr> parse_expr() {
    if (peek().has_value() && peek().value().type == TokenType::INT_LIT) {
      return NodeExpr{.int_lit = consume()}; // consuming the literal
    } else {
      return {};
    }
  }

  // This method look for the next token with an offset of 0, can be incremented
  // by an offset
  [[nodiscard]] inline std::optional<Token> peek(int offset = 0) const {
    if (m_index + offset >= m_tokens.size()) {
      return {};
    } else {
      return m_tokens.at(m_index + offset);
    }
  }
  /**
          This method consumes a token
  */
  inline Token consume() { return m_tokens.at(m_index++); }

private:
  const std::vector<Token> m_tokens;
  size_t m_index = 0;
};

#endif // PARSER_H
