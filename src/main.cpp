#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include "./main.h"


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

	if(correct_extension(filename, extension) == EXIT_SUCCESS){
		std::stringstream contents_stream;
		std::string contents;


		/* reading a file
	 	* argv[0] is the path and std::ios::in is for reading only.
		 * It will automatically close the buffer due to the scope.
		 */
		{
	       		std::fstream input(argv[0], std::ios::in);
			// reading the whole thing from the file
			contents_stream << input.rdbuf();
			contents = contents_stream.str();
		}

		std::cout << contents << std::endl;
	}

	std::cout << extension << std::endl;

	return EXIT_SUCCESS;
}
