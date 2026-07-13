#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <optional>
#include <stdatomic.h>
#include <vector>

enum class TokenType {
  IDENTIFIER,
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
  MULT_OP,

  // Data types
  INT_KEY,
  STRING_KEY,
  FLOAT_KEY,
  DOUBLE_KEY,
  BOOL_KEY,

  // Symbols
  OP_PAREN,
  CL_PAREN,
  OP_CURLY,
  CL_CURLY,

  // literals
  INT_LIT,
  STRING_LIT,
  FLOAT_LIT,
  DOUBLE_LIT,
  BOOL_LIT,

  // functions
  RETURN,
  EXIT,

  // display
  PRINTF,

  // termination of the line
  SEMICOLON,
  END_OF_LINE
};

typedef struct Token {
  TokenType type;
  std::optional<std::string> value{}; // can be Hlulani or any literal ...etc
} Token;

class Lexer {
public:
  // taking a copy of what I pass - string being passed
  inline explicit Lexer(std::string str) : m_str(std::move(str)) {}

  inline std::vector<Token> tokenize() {
    std::string current_string{};
    std::vector<Token> tokens{};

    while (peek().has_value()) {
      if (char curr_char = peek().value(); std::isalpha(curr_char)) {
        current_string.push_back(consume());

        while (peek().has_value() && (std::isalnum(peek().value()) || peek().value() == '_')) {
          current_string.push_back(consume());
        }

        // data types
        if (current_string == "int") {
          tokens.push_back({.type = TokenType::INT_KEY});
        } else if (current_string == "string") {
          tokens.push_back({.type = TokenType::STRING_KEY});
        } else if (current_string == "float") {
          tokens.push_back({.type = TokenType::FLOAT_KEY});
        } else if (current_string == "double") {
          tokens.push_back({.type = TokenType::DOUBLE_KEY});
        }
        // boolean
         else if (current_string == "bool") {
             tokens.push_back({.type = TokenType::BOOL_KEY});
        }
        //printf or print both perform the same function
        else if (current_string == "printf" || current_string == "print") {
          tokens.push_back({.type= TokenType::PRINTF});
        }
        // if statement
        else if (current_string == "if") {
          tokens.push_back({.type = TokenType::IF_KEY});
        } else if (current_string == "else") {
          tokens.push_back({.type = TokenType::ELSE_KEY});
        }
        // for loop
        else if (current_string == "for") {
          tokens.push_back({.type = TokenType::FOR_KEY});
        }
        // exit
        else if (current_string == "exit") {
          tokens.push_back({.type = TokenType::EXIT});
        } else {
          tokens.push_back({.type=TokenType::IDENTIFIER, .value = current_string});
        }

        current_string.clear(); // clearing the string
      } else if (std::isdigit(curr_char)) {
        current_string.push_back(consume());

        while (peek().has_value() &&
               std::isdigit(peek().value())) {
          current_string.push_back(consume());
        }

        // debugging
        // std::cout << current_string << std::endl;

        tokens.push_back({.type = TokenType::INT_LIT, .value = current_string});
        current_string.clear();
      } else if (std::isspace(curr_char)) {
        consume();
        continue;
      } else if (curr_char == ';') {
        tokens.push_back({.type = TokenType::SEMICOLON});
        consume(); // consume
        // std::cout << "Semicolon" << std::endl; // for debugging
      } else if (curr_char == '"') {
        consume();
        if (peek().has_value()) {
          std::string curr{};
          while (peek().has_value() && peek().value() != '"') {
            curr.push_back(consume());
          }

          // consume the close string value
          if (peek().has_value() && peek().value() == '"') {
            consume();
          }

          tokens.push_back({.type = TokenType::STRING_LIT, .value = curr});
        }
      }
      // symbols
      else if (curr_char == '{') {
        tokens.push_back({.type = TokenType::OP_CURLY});
        consume();
      } else if (curr_char == '}') {
        tokens.push_back({.type = TokenType::CL_CURLY});
        consume();
      } else if (curr_char == '(') {
        tokens.push_back({.type = TokenType::OP_PAREN});
        consume();
      } else if (curr_char == ')') {
        tokens.push_back({.type = TokenType::CL_PAREN});
        consume();
      }
      // comments
      else if ((curr_char == '/' && peek(1).has_value() && peek(1).value() == '/') ||
              (curr_char == '-' && peek(1).has_value() && peek(1).value() == '-') ||
              curr_char == '#') {
        while (peek().has_value() && peek().value() != '\n') {
          consume();
        }
      } else {
        std::cerr << "Hahaha error: symbol \'" << curr_char << "\' has been used in your code hence error." << std::endl; // error for debugging for now
        consume(); // consume to avoid infinite loop
      }
    }

    m_curr_index = 0;
    return tokens;
  }

private:
  const std::string m_str;
  int m_curr_index = 0;

  [[nodiscard]] inline std::optional<char> peek(const int &offset= 0) const {
    /**
            This method peaks characters ahead, 0 is for default and you can
       specify the offset It does not change the contents of the class hence
       const and no-discard, This is same as peek in other compilers
    */
    if (m_curr_index + offset >= m_str.size()) {
      return {};
    } else {
      return m_str.at(m_curr_index + offset);
    }
  }

  inline char consume() {
    // This is similar to consume
    return m_str.at(m_curr_index++);
  }

  inline Token tokenize_string() {
    /**
     * This method is tokenizing a string only
     */

    std::string current_string{};
    // pass the first quote
    consume();

    // pushing everything even the symbols
    while (peek().has_value()) {
      current_string.push_back(consume());
    }

    // consume the last quote
    return {.type=TokenType::STRING_LIT, .value = current_string};
  }

  inline Token tokenize_boolean() {
    /**
     * This method parses the boolean - true or false
     */
    std::string current_string{};

    while (peek().has_value() && std::isalpha(peek().value())) {
      current_string.push_back(consume());
    }

    if (current_string == "true" || current_string == "false" || current_string == "False" || current_string == "True") {
      return {.type = TokenType::BOOL_KEY, .value = current_string};
    } else {
      std::cerr << "The value stored in <bool> type is not compatible with it. change the value stored inside." << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  inline void pass_comment() {
    /**
     * This method lexers a comment
     */
    if ((peek() == '/' && peek(1).has_value() && peek(1).value() == '/') || (peek() == '-' && peek(1).has_value() && peek(1).value() == '-') || peek() == '#') {
      while (peek().has_value() && peek().value() != '\n') {
        consume();
      }
    }

    // TODO: Multiple line comments but on beta tes
    if (peek().has_value() && peek().value() == '/' && peek(1).has_value() && peek(1).value() == '*') {
      consume(); // consume /
      consume(); // *
      while (peek().has_value() && peek().value() != '*' && peek(1).has_value() && peek(1).value() != '/') {
        consume();
      }
    } else {
      std::cerr << "Can't lexer the multiple line comment" << std::endl;
      exit(EXIT_FAILURE);
    }
  }

  inline Token tokenize_integer() {
    /**
     * This method lexers the integers only not float or what's so ever
     */
    std::string current_string{};

    while (peek().has_value() && std::isdigit(peek().value())) {
      current_string.push_back(consume());
    }

    return {.type = TokenType::INT_LIT, .value = current_string};
  }

  inline Token tokenize_identifier() {
    /**
     * This method tokenize the identifier
     */
    std::string current_string{};
    if (peek().has_value() && std::isalpha(peek().value())) {
      current_string.push_back(consume());
    }

    while (peek().has_value() && (std::isalnum(peek().value()) || peek().value() == '_')) {
      current_string.push_back(consume());
    }

    return {.type = TokenType::IDENTIFIER, .value = current_string};
  }
};

#endif // LEXER_H
