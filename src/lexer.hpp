#ifndef LEXER_H
#define LEXER_H
#include <algorithm>
#include <map>
#include <optional>
#include <vector>
#include <string>

// Macros for arithmetic symbols
#define PLUS '+'
#define MINUS '-'
#define MULTIPLY '*'
#define DIVIDE '/'
#define EQUALS '='
#define MODULUS '%'

// Symbols
#define OPEN_CURLY '{'
#define CLOSE_CURLY '}'
#define OPEN_PAREN '('
#define CLOSE_PAREN ')'
#define SEMICOLON_ ';'
#define DOUBLE_QUOTE '"'

enum class TokenType {
  IDENTIFIER,
  // control flow
  IF_KEY,
  ELSEIF_KEY,
  ELSE_KEY,
  WHILE_KEY,
  FOR_KEY,

  // Arithmetic operators
  EQUALS_OP,
  PLUS_OP,
  DIVIDE_OP,
  MINUS_OP,
  MULT_OP,
  MOD_OP,
  EQUALITY_OP,

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
   explicit Lexer(std::string str) : m_str(std::move(str)) {}

   std::vector<Token> tokenize();

private:
  const std::string m_str;
  int m_curr_index = 0;

  const std::map<char, Token> operators_map = {
    {PLUS,{.type = TokenType::PLUS_OP, .value = std::string("+")}},
    {MINUS,{.type = TokenType::MINUS_OP, .value = std::string("-")}},
    {DIVIDE,{.type = TokenType::DIVIDE_OP, .value = std::string("/")}},
    {MODULUS,{.type = TokenType::MOD_OP, .value = std::string("%")}},
    {MULTIPLY,{.type = TokenType::MULT_OP, .value = std::string("*")}},
    {EQUALS, {.type = TokenType::EQUALS_OP, .value = std::string("=")}}
  };

  [[nodiscard]]  std::optional<char> peek(const int &offset= 0) const;

   char consume();

   Token tokenize_string();

   Token tokenize_boolean();

   void pass_comment(const char &curr_char);

   Token tokenize_integer();

   Token tokenize_word();

  [[nodiscard]]  bool is_comment(const char &curr_char) const;

   void tokenize_symbols(const char &curr_char, std::vector<Token> &tokens);
};

#endif // LEXER_H