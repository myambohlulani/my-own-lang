#ifndef MAIN_H
#define MAIN_H

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

#endif // MAIN_H
