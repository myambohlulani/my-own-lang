#include <iostream>
#include "./lexer.hpp"
#include <fstream>
#include <string>

int main(void) {
    // manual way of doing the lexing
    std::ifstream file("main.hlx");

    if(!file.is_open()) {
        std::cout << "Failed to open the file\n";
        return 1;
    }

    std::string line;

    while(std::getline(file, line)) {
        std::cout << line << "\n";
    }
}