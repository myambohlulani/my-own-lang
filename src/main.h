#ifndef MAIN_H
#define MAIN_H
#include <fstream>
#include <string>
#include <iterator>
#include <optional>
#include <vector>

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

// TOKEN types
enum class TokenType {
	_return,
	int_lit,
	semicolon
}

// TOKEN
typedef struct Token {
	TokenType type,
	std::optional<std::string> value;
} Token;

// Tokenizer
std::vector<Token> tokenize(const std::string &str) {
	std::vector<char> buf {}; // buffer

	for(int i = 0; i < str.length(); i++) {
		char c = str.at(i);

		// checking for keywords
		if(std::isalpha(c)) {
			buf.push_back(c);
			i++;

			while(std::isalnum(str.at(i))) {
				buf.push_back(str.at(i));
				i++;
			}

			i--;
		}

		// checking for keyword.

	}

}

#endif // MAIN_H
