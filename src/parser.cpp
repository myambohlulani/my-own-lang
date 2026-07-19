#include "./lexer.hpp"
#include "./parser.hpp"
#include <string>
#include <optional>
#include <vector>
#include <iostream>
#include <memory>
#include <variant>

[[nodiscard]]  std::optional<NodeProgram> Parser::parse() {
  /**
   * This method parse the program as a whole instead of calling the methods one by one but rather all of them depending on the tokens
   */
  NodeProgram program;
  // parsing stuff here
  while (peek().has_value()) {
    if (auto statement = parse_statement()) {
      program.statements.push_back(std::move(statement.value()));
    } else if (peek().value().type == TokenType::END_OF_FILE) {
      break; // reached the end so stop
    } else {
      std::cerr << "These statements are invalid" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  return program;
}

[[nodiscard]]  int Parser::binding_power(const TokenType &curr_type) const {
  /**
   * This method is responsible for binding power of operands
   */
  switch (curr_type) {
  case TokenType::EQUALS_OP:
    return 1;
    // binding power for +, -
  case TokenType::MINUS_OP:
  case TokenType::PLUS_OP:
    return 2;
    // binding power of *, % and /
  case TokenType::MULT_OP:
  case TokenType::DIVIDE_OP:
  case TokenType::MOD_OP:
    return 3;
  default:
    // binding power of the unknown
    std::cout << "Try implementing the binding power for current operand" << std::endl;
    return 0;
  }
}


[[nodiscard]]  std::optional<Token> Parser::peek(const int &offset) const {
  /**
   * This method look for the next token with an offset of 0, can be incremented
   * by an offset
  */
  if (m_index + offset >= m_tokens.size()) {
    return {};
  } else {
    return m_tokens.at(m_index + offset);
  }
}

 std::optional<NodeVarDeclaration> Parser::parse_variable_declaration() {
  /**
  * This method parse the data type
  */
    std::optional<NodeVarDeclaration> declaration{};

    // checking the type of the variable that is about to be created
    const bool is_keyword_type = peek().has_value() &&
        (peek().value().type == TokenType::INT_KEY ||
        peek().value().type == TokenType::STRING_KEY ||
        peek().value().type == TokenType::FLOAT_KEY ||
        peek().value().type == TokenType::DOUBLE_KEY);

    // it must have a name
    const bool contains_identifier = peek(1).has_value() && peek(1).value().type == TokenType::IDENTIFIER;
    const bool op_has_curly = peek(2).has_value() && peek(2).value().type == TokenType::OP_CURLY;

    // has keyword followed by name and open curly i.e string name{
    if (is_keyword_type && contains_identifier && op_has_curly) {
      Token type_token = consume();
      Token ident_token = consume();
      // consume curly brace -> open
      consume();

      // parsing string
      if (type_token.type == TokenType::STRING_KEY) {
        if (auto str_value = parse_string()) {
          declaration = NodeVarDeclaration{.data_type = type_token, .var_name = ident_token, .value = str_value.value()};
        } else {
          std::cerr << "Your variable type is String but the value stored is different" << std::endl;
          exit(EXIT_FAILURE);
        }
      }
      // then it is not a string it either int/float or double
      else {
        if (auto int_value = parse_expr()) {
          declaration = NodeVarDeclaration{.data_type = type_token, .var_name = ident_token, .value = std::move(int_value.value())};
        } else {
          // add a default value of 0 since it is an integer
          std::cerr << "The value that should be stored is int" << std::endl;
          exit(EXIT_FAILURE);
        }
      }

      // close curly
      if (peek().has_value() && peek().value().type == TokenType::CL_CURLY) {
        consume();
      } else {
        std::cerr << "There is no close curly brace for the creation of data type" << std::endl;
        exit(EXIT_FAILURE);
      }

      if (peek().has_value() && peek().value().type == TokenType::SEMICOLON) {
        consume();
      } else {
        std::cerr << "There is no semicolon for your variable creation" << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    return declaration;
}

 std::optional<NodeExit> Parser::parse_exit() {
  /**
   * This method parses the exit statement
   */
  std::optional<NodeExit> exit_node {};
  if (peek().value().type == TokenType::EXIT && peek(1).has_value() && peek(1).value().type == TokenType::OP_PAREN) {
    consume(); // consume exit
    consume(); // consume the next open parentheses

    // TODO: check for the value
    if (auto node_expr = parse_expr()) {
      exit_node  = NodeExit {.expr = std::move(node_expr.value())};
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

  return exit_node;
}

 std::optional<NodeStatement> Parser::parse_statement() {
  /**
   * This method parses the statement, any statement
   */
  // checking for exit
  const bool &is_open_parentheses = peek(1).has_value() && peek(1).value().type == TokenType::OP_PAREN;
  if ( peek().has_value() && peek().value().type == TokenType::EXIT && is_open_parentheses) {
    if (auto node = parse_exit()) {
      return NodeStatement{.var = std::move(node.value())};
    }
  } else if (peek().has_value() && peek().value().type == TokenType::PRINTF && is_open_parentheses) {
    if (auto node = parse_printf()) {
      return NodeStatement{.var = std::move(node.value())};
    }
  }
  // variable declaration
  else if (auto node = parse_variable_declaration()) {
    return NodeStatement{.var = std::move(node.value())};
  }

  return {};
}

// parsing a string
 std::optional<NodeStr> Parser::parse_string() {
  if (peek().has_value() && peek().value().type == TokenType::STRING_LIT) {
    return NodeStr{.string_lit = consume()};
  }

  return {};
}

 std::optional<NodeExpr> Parser::parse_expr(const int &min_binding_pow) {
  /**
  * This method parsers and expression
  * Using the Pratt Parsing method
  */

  //parse prefix operands
  NodeExpr left = prefix_parser();

  while (true) {
    auto curr_token = peek();

    // there is no next token
    if (!curr_token.has_value()) {
      break;
    }

    // same binding power or less -> skip
    if (const int &curr_binding_power = binding_power(curr_token.value().type); curr_binding_power <= min_binding_pow) {
      break;
    }

    left = parse_infix_expr(std::move(left), consume());

  }

  return left;
}

 NodeExpr Parser::prefix_parser() {
  /**
   * Implementation of prefix parser -> Pratt Parsing
   */
  const Token &curr_token = consume();

  // num + 11 + (11 - 10)
  switch (curr_token.type) {
    // identifier then
  case TokenType::IDENTIFIER:
    return NodeExpr{NodeIdentExpr{curr_token}};
    // TODO: I have to implement one for floating points
    // case TokenType::FLOAT_LIT:
    // case TokenType::DOUBLE_LIT:
  case TokenType::INT_LIT:
    return NodeExpr{NodeLiteral{curr_token}};
    // TODO: Parse left paren
  default:
    std::cerr << "Prefix - Parser error" << std::endl;
    return NodeExpr{};
  }
}

 std::optional<NodePrintf> Parser::parse_printf() {
  /**
   * This method parses the printf statement
  */
  std::optional <NodePrintf> printf_node {};
  if (peek().value().type == TokenType::PRINTF && peek(1).has_value() && peek(1).value().type == TokenType::OP_PAREN) {
    consume(); // printf
    consume(); // (

    if (peek().has_value() && peek().value().type == TokenType::STRING_LIT) {
      if (auto node_str_expr = parse_string()) {
        printf_node = NodePrintf {.expr = std::move(node_str_expr.value())};
      } else {
        std::cerr << "Did you forget to include a string or value inside your print?" << std::endl;
        exit(EXIT_FAILURE);
      }
    } else if (auto node_expr = parse_expr()) {
      printf_node  = NodePrintf {.expr = std::move(node_expr.value())};
    } else {
      std::cerr << "Did you forget to include a string or value inside your print?" << std::endl;
      exit(EXIT_FAILURE);
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

  return printf_node;
}

[[nodiscard]]  NodeExpr Parser::parse_infix_expr(NodeExpr left, const Token &operand) {
  /**
   * This method parses the expr after getting the expr that begins
   */
  const int &curr_binding_power = binding_power(operand.type);
  auto right = parse_expr(curr_binding_power);

  // guard in case the value is not found
  if (!right.has_value()) {
    std::cerr << "You inserted an operator but it does not have an expression following it." << std::endl;
    exit(EXIT_FAILURE);
  }

  return NodeExpr {
    .value = NodeBinaryExpr{
      .operand_lit = operand,
      .left = std::make_unique<NodeExpr>(std::move(left)),
      .right = std::make_unique<NodeExpr>(std::move(right.value()))}
  };
}

 Token Parser::consume() {
  /**
  * This method consumes a token
  */
  return m_tokens.at(m_index++);
}