#include "./main.hpp"
#include "./codegen.hpp"
#include "./lexer.hpp"
#include "./parser.hpp"
#include "./x86_codegen.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc == 2) {
    // has radium and the file
    // checking for correct extension
    std::string extension;
    const std::string filename = argv[1];
    // const std::string filename = "../hello.hlx";

    // checking for correct extension
    if (correct_extension(filename, extension) == EXIT_SUCCESS) {
      std::string contents = read_file_contents(filename); // reading the file

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

      // generating the code into mips
      const std::string output_for_mips = generator-> generate();

      // TODO instead of login failure, i want to log success and make sure that the assembly code representation is generated for exit
      if (output_for_mips.empty()) {
        write_contents_into_a_file(output_for_mips);
        exit(EXIT_SUCCESS);
      }

      // writing for mips
      write_contents_into_a_file(output_for_mips);

      // generating code into x86 assembly
      const CodeGenerator cod(tree.value());
      const CodeGenerator *codegen = &cod;

      const std::string output_for_x86 = codegen -> generate();

      // writing for x86
      std::string default_output_path = "./bin/out.s";
      if (output_for_x86.empty()) {
        // generate default exit if the file is empty
        write_contents_into_a_file(output_for_x86, default_output_path, filename);
        exit(EXIT_SUCCESS);
      }

      write_contents_into_a_file(output_for_x86, default_output_path, filename);

      // link x86
      assemble_and_link_x86();
    }
  }
  else {
    // it does not have -o for  file path ...etc
    std::cerr << "Incorrect usage of the compiler." << std::endl;
    std::cerr << "The correct usage is: radium <filename.hlx> or <filename.hl>" << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
