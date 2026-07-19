#ifndef PARSER_H
#define PARSER_H

#include "./lexer.hpp"
#include <optional>
#include <variant>
#include <memory>

// forward declaration of the NodeExpr
struct NodeExpr;

// This is for integers/float/double literal
struct NodeLiteral {
  Token int_lit;
};

// This is responsible for previous declaration of a certain variable
struct NodeIdentExpr {
  Token ident;
};

// Responsible for Expr of operands
struct NodeBinaryExpr {
  Token operand_lit;
  std::unique_ptr<NodeExpr> left;
  std::unique_ptr<NodeExpr> right;
};

// expr will either be ref or literal
struct NodeExpr {
  std::variant<NodeLiteral, NodeIdentExpr, NodeBinaryExpr> value;
};

// string
struct NodeStr {
  Token string_lit;
};

// exit statement
struct NodeExit {
  NodeExpr expr;
};

// print statement
struct NodePrintf {
  std::variant<NodeExpr, NodeStr> expr;
};

/**
 * This is responsible for declaration of variables
 */
struct NodeVarDeclaration {
  Token data_type; // type
  Token var_name; // variable name
  std::variant<NodeExpr, NodeStr> value; // the value the type contains
};

/**
 * This is responsible for any statement in the code
 */
struct NodeStatement {
  std::variant<NodeExit, NodePrintf, NodeVarDeclaration> var;
};

// This will contain all the statements that will be in the program
struct NodeProgram {
  std::vector<NodeStatement> statements;
};

class Parser {
public:
  /**
    * Works similar to the Lexer,but goes through each and every token
   * instead of char
  */
   explicit Parser(const std::vector<Token> &tokens) : m_tokens(tokens) {}

  [[nodiscard]]  std::optional<NodeProgram> parse();

private:
  const std::vector<Token> m_tokens;
  size_t m_index = 0;

   std::optional<NodeExit> parse_exit();

   Token consume();

   std::optional<NodePrintf> parse_printf();

   std::optional<NodeVarDeclaration> parse_variable_declaration();

   std::optional<NodeStatement> parse_statement();

   std::optional<NodeStr> parse_string();

  [[nodiscard]]  std::optional<Token> peek(const int &offset = 0) const;

   std::optional<NodeExpr> parse_expr(const int &min_binding_pow = 0);

  [[nodiscard]]  NodeExpr parse_infix_expr(NodeExpr left, const Token &operand);

  [[nodiscard]]  int binding_power(const TokenType &curr_type) const;

    NodeExpr prefix_parser();
};

#endif // PARSER_H