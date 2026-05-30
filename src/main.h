#ifndef MAIN_H
#define MAIN_H
#include <fstream>
#include <string>
#include <iterator>

// CHECKING FOR FILE EXTENSION
int correct_extension(std::string filename, std::string extension) {
        extension = filename.substr(filename.size() - 3);

        if(!(extension == "hlx" || extension == ".hl") ) {
                std::cerr << "The extension of the file does not match." << std::endl;
                std::cerr << "File has been given: " << extension << " but expected: .hlx or .hl" << std::endl;

                return EXIT_FAILURE;
        }


        return EXIT_SUCCESS;
}

// Reading all contents of a file at once
std::string read_file_contents(std::string filename) {
	std::ifstream file(filename);

	if(!file.is_open()) {
		std::cout << "file faile to open" << std::endl;
		return "";
	}

	std::ostringstream ss;

	ss << file.rdbuf();

	return ss.str();
}

#endif // MAIN_H
