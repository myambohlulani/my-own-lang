#ifndef PARSER_H
#define PARSER_H

#include "./lexer.hpp"
#include <optional>

// this will hold the integer
struct NodeExprIntLit {
  Token int_lit;
};

// this will hold - indentifier
struct NodeExprIndent {
  Token ident;
};

// will either contain expr or identifier
struct NodeExpr {
  std::variant<NodeExprIntLit, NodeExprIndent> var; // can be one of either
};

struct NodeStatement {
  std::variant<NodeStmtExit, NodeExprIdent> var;
};

// exit(0) ..etc
struct NodeStmtExit {
  NodeExpr expr;
};

class Parser {
  /**
          Works similar to the Lexer,but goes through each and every token
     instead of char
  */
public:
  inline explicit Parser(std::vector<Token> tokens)
      : m_tokens(std::move(tokens)) {}

  /**
          This method parses the exit statement with parentheses.
  */
  inline std::optional<NodeExit> parse() {
    std::optional<NodeStmtExit> exit_node;

    while (peek().has_value()) {
      // first token is exit and the next is open paren hence peek 1
      if (peek().value().type == TokenType::EXIT && peek(1).has_value() &&
          peek(1).value().type == TokenType::OP_PAREN) {
        consume(); // consuming exit
        // consume the open paren too
        consume();
        if (auto node_expr = parse_expr()) {
          exit_node = NodeExit{.expr = node_expr.value()};
        } else {
          std::cerr << "Invalid expression" << std::endl;
          exit(EXIT_FAILURE);
        }

        // taking out the close paren
        if (peek().has_value() && peek().value().type == TokenType::CL_PAREN) {
          consume();
        } else {
          std::cerr << "There is no close parentheses in your exit."
                    << std::endl;
        }

        // semi colon
        if (peek().has_value() && peek().value().type == TokenType::SEMICOLON) {
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

  inline std::optional<NodeExpr>
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
