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

inline std::optional<char> Lexer::peek(const int &offset) const {
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

inline char Lexer::consume() {
  /**
   * This method is responsible for passing the current character
   * It returns the current character and moves to the next character in the lexer
   */
  return m_str.at(m_curr_index++);
}
