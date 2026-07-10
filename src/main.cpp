#include "./main.hpp"
#include "./codegen.hpp"
#include "./lexer.hpp"
#include "./parser.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    // we have more than 2 or less than 2 arguments
    std::cerr << "Incorrect usage of the compiler." << std::endl;
    std::cerr << "The correct usage is: radium <filename.hlx> or <filename.hl>"
              << std::endl;

    return EXIT_FAILURE;
  }

  // checking for correct extension
  std::string extension;
  const std::string filename = argv[1];
  // const std::string filename = "../hello.hlx"; // for debuging
  std::string contents;

  // checking for correct extension
  if (correct_extension(filename, extension) == EXIT_SUCCESS) {
    contents = read_file_contents(filename); // reading the file

    // did not read anything so it is considered a failure.
    if ((contents == "")) {
      return EXIT_FAILURE;
    }

    // printing the content i just read. for debugging purposes
    // std::cout << contents << std::endl;

    // start lexing or converting into tokens
    Lexer lex(std::move(contents));
    Lexer *lexer = &lex;

    const std::vector<Token> tokens = lexer->tokenize(); // tokens

    // Parser
    Parser par(tokens);
    Parser *parser = &par;
    const std::optional<NodeProgram> tree = parser -> parse();

    if (!tree.has_value()) {
      std::cerr << "The program is invalid haha:-)" << std::endl;
      exit(EXIT_FAILURE);
    }

    // generator
    const Generator gen(tree.value()); // TODO: Change this
    const Generator *generator = &gen;

    // generating the code
    const std::string strs = generator-> generate();
    if (strs.empty()) {
      exit(EXIT_FAILURE);
    }

    write_contents_into_a_file(strs);
  }

  return EXIT_SUCCESS;
}
