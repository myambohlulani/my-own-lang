#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include "./main.hpp"
#include "./lexer.hpp"
#include "./parser.hpp"
#include "./codegen.hpp"

int main(int argc, char* argv[]) {
	if(argc != 2 ) {
		// we have more than 2 or less than 2 arguments
		std::cerr << "Incorrect usage of the compiler." << std::endl;
		std::cerr << "The correct usage is: radium <filename.hlx> or <filename.hl>" << std::endl;

		return EXIT_FAILURE;
	}

	// checking for correct extension
	std::string extension;
	std::string filename = argv[1];
	std::string contents;

	// checking for correct extension 
	if(correct_extension(filename, extension) == EXIT_SUCCESS){
		contents = read_file_contents(filename); // reading the file

		// did not read anything so it is considered a failure.
		if((contents == "")) {
			return EXIT_FAILURE;
		} 
		
		// printing the content i just read. for debugging purposes
		//std::cout << contents << std::endl;

		// start lexing or converting into tokens
		Lexer lex(std::move(contents));
		Lexer* lexer = &lex;

		std::vector<Token> tokens = lexer -> tokenize(); // tokens
		
		// Parser
		Parser par(std::move(tokens));
		Parser* parser = &par;
		std::optional<NodeExit> tree = parser -> parse();

		if(!tree.has_value()) {
			std::cerr << "there exists no exit statement" << std::endl;
			exit(EXIT_FAILURE);
		}

		// generator
		Generator gen(tree.value());
		Generator* generator = &gen;

		// generating the code
		std::string strs = generator -> generate();
		write_contents_into_a_file(strs);
	}	


	return EXIT_SUCCESS;
}
