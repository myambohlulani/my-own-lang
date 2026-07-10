#ifndef PARSER_H
#define PARSER_H

#include "./lexer.hpp"
#include <optional>
#include <variant>

struct NodeExpr {
  Token int_lit;
};

struct NodeExit {
  NodeExpr expr;
};

struct NodePrintf {
  NodeExpr expr;
};

struct NodeIdent {
  NodeExpr expr;
  Token name;
};

struct NodeStr {
  Token string_lit;
};

struct NodeStatement {
  std::variant<NodeExit, NodePrintf> var;
};

// This will contain all the stetements that will be in the program
struct NodeProgram {
  std::vector<NodeStatement> statements;
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

  /** This method parses the printf statement
   */
  inline std::optional<NodePrintf> parse_printf() {
    std::optional <NodePrintf> printf_node {};

    while (peek().has_value()) {
      if (peek().value().type == TokenType::PRINTF && peek(1).has_value() && peek(1).value().type == TokenType::OP_PAREN) {
        consume(); // printf
        consume(); // (

        if (const auto node_expr = parse_expr()) {
          printf_node  = NodePrintf {.expr = node_expr.value()};
        }

        // close parentheses
        if (peek().has_value() && peek().value().type == TokenType::CL_PAREN) {
          consume();
        } else {
          std::cerr << "There is no close parentheses for your printf" << std::endl;
          exit(EXIT_FAILURE);
        }

        if (peek().has_value() && peek().value().type == TokenType::SEMICOLON) {
          consume();
        } else {
          std::cerr << "There is no semicolon for termination of printf" << std::endl;
        }
      }
    }

    return printf_node;
  }

  /**
   * This method parse the data type
   * /
   */
  inline std::optional<NodeIdent> parse_data_type() {
    std::optional<NodeIdent> data_type {};

    while (peek().has_value()) {
      if ((peek().value().type == TokenType::INT_KEY ||
        peek().value().type == TokenType::STRING_KEY ||
        peek().value().type == TokenType::FLOAT_KEY ||
        peek().value().type == TokenType::DOUBLE_KEY) && peek(1).has_value() && peek(1).value().type == TokenType::OP_CURLY) {
        consume();

        if (const auto int_data_type_expr = parse_expr()) {
          data_type  = NodeIdent {.expr = int_data_type_expr.value()};
        }
        // float
        else if (const auto float_data_type_expr = parse_expr()) {
          data_type  = NodeIdent {.expr = float_data_type_expr.value()};
        }
        // double
        else if (const auto double_data_type_expr = parse_expr()) {
          data_type  = NodeIdent {.expr = double_data_type_expr.value()};
        }
        // string


        // check for closed curly brace

        if (peek().has_value() && peek().value().type == TokenType::CL_CURLY) {
          consume();
        } else {
         std::cout << "There is no close parentheses in the data_type" << std::endl;
        }

        if (peek().has_value() && peek().value().type == TokenType::SEMICOLON) {
          consume();
        } else {
          std::cout << "There is no semicolon in the datatype" << std::endl;
        }
      }
    }

    return data_type;
  }

  /**
     This method parsers and expression
   */
  inline std::optional<NodeExpr> parse_expr() {
    if (peek().has_value() && peek().value().type == TokenType::INT_LIT) {
      return NodeExpr{.int_lit = consume()}; // consuming the literal
    } else if (peek().has_value() && peek().value().type == TokenType::FLOAT_LIT) {
      return NodeExpr{.int_lit = consume()}; // consuming the literal
    } else if (peek().has_value() && peek().value().type == TokenType::DOUBLE_LIT) {
      return NodeExpr{.int_lit = consume()}; // consuming the literal
    }

    return {};
  }

  // parsing a string
  inline std::optional<NodeStr> parse_string() {
    if (peek().has_value() && peek().value().type == TokenType::STRING_LIT) {
      return NodeStr{.string_lit = consume()};
    }

    return {};
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
