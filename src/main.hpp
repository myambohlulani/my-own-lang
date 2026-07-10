#ifndef MAIN_H
#define MAIN_H

#include <filesystem>
#include <fstream>
#include <iterator>
#include <optional>
#include <string>
#include <vector>
#include <iostream>

// CHECKING FOR FILE EXTENSION
inline int correct_extension(const std::string &filename, std::string extension) {
  extension = filename.substr(filename.size() - 3);

  if (!(extension == "hlx" || extension == ".hl")) {
    std::cerr << "The extension of the file does not match." << std::endl;
    std::cerr << "File has been given: " << extension
              << " but expected: .hlx or .hl" << std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

// Reading all contents of a file at once
inline std::string read_file_contents(const std::string &filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    std::cout << "file " << filename << " failed to open" << std::endl;

    return "";
  }

  std::ostringstream ss;

  ss << file.rdbuf();

  return ss.str();
}

inline std::filesystem::path derive_output_path(const std::string& source_path) {
  std::filesystem::path p(source_path);
  p.replace_extension(".asm");
  return p;
}

inline void write_contents_into_a_file(const std::string &contents, const std::optional<std::string> &output_path = std::nullopt,
const std::optional<std::string> &source_path = std::nullopt) {

  std::filesystem::path p;

  if (output_path.has_value()) {
    p = *output_path;
  } else if (source_path.has_value()) {
    p = derive_output_path(*source_path);
  } else {
    p = "./bin/out.asm";
  }

  if (p.has_parent_path()) {
    std::filesystem::create_directories(p.parent_path());
  }

  std::ofstream file(p);
  if (!file) {
    std::cerr << "Failed to create the file\n";
    exit(EXIT_FAILURE);
  }

  file << contents;
}

#endif // MAIN_H
