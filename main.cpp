#include <iostream>
#include <filesystem>
#include <fstream>

#include "input-content.hpp"

static void print_usage() {
  std::cout << "Usage: ./demo-splitter <path-to-directory-with-input-files> <path-to-output-file>" << std::endl;
}

int main(int argc, char** argv) {
  if (argc != 3) {
    print_usage();
    return 0;
  }

  const std::filesystem::path input_dir_path{ argv[1] };
  if (!std::filesystem::is_directory(input_dir_path)) {
    std::cerr << input_dir_path << " must be a directory!" << std::endl;
    return 0;
  }

  std::ofstream output_file;
  output_file.exceptions( std::ofstream::badbit | std::ofstream::failbit );
  try {
    output_file.open(argv[2]);
    for (const auto& entry : std::filesystem::directory_iterator{ input_dir_path }) {
      if (entry.is_regular_file()) {
        // loading content from input file
        InputContent input_content{ entry.path() };
        output_file << "[Имя файла " << input_content.input_file_name << "]:"; 
        for (auto str : input_content) {
          output_file << "\n" << str;
        }
        output_file << std::endl;
        std::cout << "Processed file: " << input_content.input_file_name << std::endl;
      }
    }
    output_file.close();
  } catch (std::ofstream::failure e) {
    std::cerr << "Error while writing to " << argv[2] << ": " << e.what() << std::endl;
  }

  return 0;
}