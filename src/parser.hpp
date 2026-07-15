#ifndef PARSER_H
#define PARSER_H

#include "./lexer.hpp"
#include <optional>
#include <variant>

// This is for integers/float/double literal
struct NodeLiteral {
  Token int_lit;
};

// This is responsible for previous declaration of a certain variable
struct NodeIdentExpr {
  Token ident;
};

// expr will either be ref or literal
struct NodeExpr {
  std::variant<NodeLiteral, NodeIdentExpr> value;
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
struct NodeVarDeclar {
  Token data_type; // type
  Token var_name; // variable name
  std::variant<NodeExpr, NodeStr> value; // the value the type contains
};

// This is responsible for if-clause
// struct NodeIfstmt {
//   std::variant<NodeExpr, bool, Token> var;
// };

/**
 * This is responsible for any statement in the code
 */
struct NodeStatement {
  std::variant<NodeExit, NodePrintf, NodeVarDeclar> var;
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

  /**
   * This method parse the program as a whole instead of calling the methods one by one but rather all of them depending on the tokens
   */
  [[nodiscard]] inline std::optional<NodeProgram> parse() {
    NodeProgram program;
    // parsing stuff here
    while (peek().has_value()) {
      if (const auto statement = parse_statement()) {
        program.statements.push_back(statement.value());
      } else if (peek().value().type == TokenType::END_OF_FILE) {
        break; // reached the end so stop
      } else {
        std::cerr << "These statements are invalid" << std::endl;
        exit(EXIT_FAILURE);
      }
    }

    return program;
  }

private:
  const std::vector<Token> m_tokens;
  size_t m_index = 0;

  /**
   * This method parses the exit statement
   */
  inline std::optional<NodeExit> parse_exit() {
    std::optional<NodeExit> exit_node {};
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

    return exit_node;
  }

  /** This method parses the printf statement
  */
  inline std::optional<NodePrintf> parse_printf() {
    std::optional <NodePrintf> printf_node {};
    if (peek().value().type == TokenType::PRINTF && peek(1).has_value() && peek(1).value().type == TokenType::OP_PAREN) {
      consume(); // printf
      consume(); // (

      if (const auto node_expr = parse_expr()) {
        printf_node  = NodePrintf {.expr = node_expr.value()};
      } else if (const auto node_str_expr = parse_string()) {
        printf_node = NodePrintf {.expr = node_str_expr.value()};
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

  /**
  * This method parse the data type
  * /
  */
  inline std::optional<NodeVarDeclar> parse_variable_declar() {
    std::optional<NodeVarDeclar> declaration{};

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
        if (const auto str_value = parse_string()) {
          declaration = NodeVarDeclar{.data_type = type_token, .var_name = ident_token, .value = str_value.value()};
        } else {
          std::cerr << "Your variable type is String but the value stored is different" << std::endl;
          exit(EXIT_FAILURE);
        }
      }
      // then it is not a string it either int/float or double
      else {
        if (auto int_value = parse_expr()) {
          declaration = NodeVarDeclar{.data_type = type_token, .var_name = ident_token, .value = int_value.value()};
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

  /**
   * This method parses the statement, any statement
   */
  inline std::optional<NodeStatement> parse_statement() {
    // checking for exit
    if (const bool &is_open_parentheses = peek(1).has_value() && peek(1).value().type == TokenType::OP_PAREN; peek().has_value() && peek().value().type == TokenType::EXIT && is_open_parentheses) {
      if (auto node = parse_exit()) {
        return NodeStatement{.var = node.value()};
      }
    }

    else if (peek().has_value() && peek().value().type == TokenType::PRINTF && is_open_parentheses) {
      if (auto node = parse_printf()) {
        return NodeStatement{.var = node.value()};
      }
    }
    // variable declaration
    else if (auto node = parse_variable_declar()) {
      return NodeStatement{.var = node.value()};
    }
    // parsing if statements
    else if (peek().has_value() && peek().value().type == TokenType::IF_KEY) {
      return NodeStatement{.var = node.value()};
    }
    // parsing else statement
    else if (peek().has_value() && peek().value().type == TokenType::ELSE_KEY) {
      return NodeStatement{.var = node.value()};
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

  /**
        This method consumes a token
  */
  inline Token consume() { return m_tokens.at(m_index++); }

  // This method look for the next token with an offset of 0, can be incremented
  // by an offset
  [[nodiscard]] inline std::optional<Token> peek(const int &offset = 0) const {
    if (m_index + offset >= m_tokens.size()) {
      return {};
    } else {
      return m_tokens.at(m_index + offset);
    }
  }

  /**
   This method parsers and expression
 */
  inline std::optional<NodeExpr> parse_expr() {
    if (peek().has_value() && (peek().value().type == TokenType::INT_LIT || peek().value().type == TokenType::FLOAT_LIT || peek().value().type == TokenType::DOUBLE_LIT)) {
      return NodeExpr{.value = NodeLiteral{.int_lit = consume()}};
    }
    // for identifier
    else if (peek().has_value() && peek().value().type == TokenType::IDENTIFIER) {
      return NodeExpr{.value = NodeIdentExpr{.ident = consume()}};
    }

    return {};
  }

  // inline std::optional<NodeIfstmt> parse_if_stament() {
  //   /**
  //    * This method tries to parse the if statement
  //    */
  //   // TODO: complete this and make it work
  //   std::optional<NodeIfstmt> if_stmt;
  //
  //   if (peek().has_value() && peek().value().type == TokenType::IF_KEY) {
  //     if (peek(1).has_value() && peek(1).value().type == TokenType::OP_PAREN) {
  //       consume();
  //       consume();
  //       // parse what is within the statement
  //       if (peek().has_value() && peek().value().type == TokenType::BOOL_LIT && (peek().value().value == "true" || peek().value().value == "false")) {
  //         return NodeIfstmt{.var = consume()}; // taking that true or false
  //       }
  //     }
  //   }
  //
  //   return if_stmt;
  // }

  [[nodiscard]] inline int binding_power(const TokenType &curr_type) const {
    /**
     * This method is responsible for binding power of operands
     */
    switch (curr_type) {
    case TokenType::EQUALS_OP:
        return 1;
      break;
      // binding power for +, -
    case TokenType::MINUS_OP:
    case TokenType::PLUS_OP:
      return 2;
      break;
    // binding power of *, % and /
    case TokenType::MULT_OP:
    case TokenType::DIVIDE_OP:
    case TokenType::MOD_OP:
      return 3;
      break;
    default:
      // binding power of the unknown
      std::cout << "Try implementing the binding power for current operand" << std::endl;
      return 0;
    }
  }
};

#endif // PARSER_H