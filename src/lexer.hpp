#ifndef LEXER_H
#define LEXER_H
#include <iostream>
#include <optional>
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
  END_OF_FILE
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
      // starts with alphabet
      if (const char curr_char = peek().value(); std::isalpha(curr_char)) {
        tokens.push_back(tokenize_word());
      }
      // value
      else if (std::isdigit(curr_char)) {
        tokens.push_back(tokenize_integer());
      }
      // space
      else if  (std::isspace(curr_char)) {
        consume();
      }
      // string
      else if (curr_char == '"') {
        tokens.push_back(tokenize_string());
      }
      // starts with // or # or -- for comments
      else if (is_comment(curr_char)) {
        pass_comment(curr_char);
      } // symbols
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
      } else if (curr_char == ';') {
        tokens.push_back({.type = TokenType::SEMICOLON});
        consume(); // consume
        // std::cout << "Semicolon" << std::endl; // for debugging
      } else {
        std::cerr << "Hahaha error: symbol \'" << curr_char << "\' has been used in your code hence error." << std::endl; // error for debugging for now
        consume(); // consume to avoid infinite loop
      }
    }
      tokens.push_back({.type = TokenType::END_OF_FILE}); // reached the end
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

  inline void pass_comment(const char &curr_char){
    /**
     * This method lexers a comment
     */

    if (curr_char == '#') {
      while (peek().has_value() && peek().value() != '\n') {
        consume();
      }
    } else if (curr_char == '/' && peek(1).has_value() && peek(1).value() == '/') {
      while (peek().has_value() && peek().value() != '\n') {
        consume();
      }
    } else if (curr_char == '-' && peek(1).has_value() && peek(1).value() == '-') {
      while (peek().has_value() && peek().value() != '\n') {
        consume();
      }
    } else {
      std::cerr << "Comments aren't being lexered haha:-)" << std::endl;
      exit(EXIT_FAILURE);
    }

    // TODO: Multiple line comments
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

  inline Token tokenize_word() {
    /**
     * This method tokenize the words
     */
    std::string current_string{};
    if (peek().has_value() && std::isalpha(peek().value())) {
      current_string.push_back(consume());
    }

    // for identifier or keywords
    while (peek().has_value() && (std::isalnum(peek().value()) || peek().value() == '_')) {
      current_string.push_back(consume());
    }

    if (current_string == "exit") {
      return {.type = TokenType::EXIT};
    } else if (current_string == "if") {
      return {.type = TokenType::IF_KEY};
    } else if (current_string == "else") {
      return {.type = TokenType::ELSE_KEY};
    } else if (current_string == "while") {
      return {.type = TokenType::WHILE_KEY};
    } else if (current_string == "for") {
      return {.type = TokenType::FOR_KEY};
    } else if (current_string == "return") {
      return {.type = TokenType::RETURN};
    } else if (current_string == "printf" || current_string == "print") {
      return {.type = TokenType::PRINTF, .value = current_string};
    }
    else if (current_string == "bool") {
      return {.type = TokenType::BOOL_KEY, .value = current_string};
    }
    else if (current_string == "true" || current_string == "false" || current_string == "True" || current_string == "False") {
      return {.type = TokenType::BOOL_LIT, .value = current_string};
    } else if (current_string == "int") {
      return {.type = TokenType::INT_KEY, .value = current_string};
    } else if (current_string == "string" || current_string == "String") {
      return {.type = TokenType::STRING_KEY, .value = current_string};
    }

    return {.type = TokenType::IDENTIFIER, .value = current_string};
  }

  [[nodiscard]] inline bool is_comment(const char &curr_char) const {
    /**
     * This method checks if it is the start of the comment
     */
    if (curr_char == '#') {
      return true;
    }

    if (const auto &next_char = peek(1); (next_char == '/' && curr_char == '/') || (curr_char == '-' && next_char == '-') ) {
      return true;
    }

    return false;
  }
};

#endif // LEXER_H
