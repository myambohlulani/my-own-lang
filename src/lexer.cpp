#include "lexer.hpp"

inline std::vector<Token> Lexer::tokenize() {
  /**
   * This is a tokenizer for the file in hand
   */
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
    // starts with // or # or -- for comments
    else if (is_comment(curr_char)) {
      pass_comment(curr_char);
    }
    // space
    else if  (std::isspace(curr_char)) {
      consume();
    }
    else {
      // tokenize known symbols
      tokenize_symbols(curr_char, tokens);
    }
  }
  tokens.push_back({.type = TokenType::END_OF_FILE}); // reached the end
  m_curr_index = 0;
  return tokens;
}