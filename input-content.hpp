#ifndef INPUT_CONTENT_H
#define INPUT_CONTENT_H

#include <string>
#include <vector>
#include <filesystem>
#include <fstream>

struct InputContentIterator {
  InputContentIterator(std::string input_string, std::vector<std::string> delimiters);
  bool operator!=(size_t pos) const;
  std::string operator*() const;
  InputContentIterator& operator++();

private:
  std::string input_string;
  std::vector<std::pair<std::string, size_t>> delimiters_positions;
  size_t start_position{};
  size_t end_position{};
};

class InputContent {
  std::string input_string;
  std::vector<std::string> delimiters;

public:
  InputContent(std::filesystem::path input_path);
  InputContentIterator begin() const;
  size_t end() const; 
};

#endif