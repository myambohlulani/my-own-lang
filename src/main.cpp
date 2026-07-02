#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include "./main.h"
#include "./lexer.hpp"


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
		
		// printing the content i just read.
		std::cout << contents << std::endl;

		// start lexing or converting into tokens
	}	


	return EXIT_SUCCESS;
}
