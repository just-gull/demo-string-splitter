#include "input-content.hpp"

// InputContent implementation
InputContent::InputContent(std::filesystem::path input_path) : input_file_name{ input_path.filename() } {
  std::ifstream input_file;
  input_file.open(input_path);
  // читаем основную строку из файла
  std::getline(input_file, input_string);
  // читаем разделители
  std::string delimiter;
  while (std::getline(input_file, delimiter)) {
    delimiters.push_back(delimiter);
  }
  input_file.close();
}

InputContentIterator InputContent::begin() const {
  return InputContentIterator{ input_string, delimiters };
}

size_t InputContent::end() const {
  return input_string.length();
}


// InputContentIterator implementation
InputContentIterator::InputContentIterator(std::string input_string, std::vector<std::string> delimiters) : input_string{ input_string } {
  // calculating end position
  auto min_value = input_string.length();
  for (auto& delimiter : delimiters) {
    auto position = input_string.find(delimiter);
    if (position != std::string::npos) {
      delimiters_positions.push_back(std::make_pair(delimiter, position));
      if (position < min_value) { 
        min_value = position; 
      }
    }
  }
  end_position = min_value;
  // если строка начинается с одного из разделителей, то увеличиваем итератор
  if (end_position == 0) ++(*this);
}

bool InputContentIterator::operator!=(size_t pos) const {
  return pos != start_position;
}

std::string InputContentIterator::operator*() const {
  return input_string.substr(start_position, end_position - start_position);
}

InputContentIterator& InputContentIterator::operator++() {
  // ищем новую стартовую позицию
  size_t next_start_position = end_position;
  // находим текущие активные разделители, их может быть больше одного
  // и рассчитываем следующую стартовую позицию как текущую конечную позицию + размер самого большого разделителя
  for (auto& delimiter_position : delimiters_positions) {
    if (delimiter_position.second == end_position) {
      auto delimiter_end_position = end_position + delimiter_position.first.length();
      if (delimiter_end_position > next_start_position) {
        next_start_position = delimiter_end_position;
      }
    }
  }

  // проверяем попали ли другие разделители в новый диапазон (end_position, next_start_position] 
  // и если попали, то при необходимости увеличиваем диапазон
  bool found;
  do {
    found = false;
    for (auto& delimiter_position : delimiters_positions) {
      if (delimiter_position.second != std::string::npos && delimiter_position.second <= next_start_position) {
        auto delimiter_end_position = delimiter_position.second + delimiter_position.first.length();
        if (delimiter_end_position > next_start_position) {
          next_start_position = delimiter_end_position;
          found = true;
        }
      }
    }
  } while (found); // повторяем до тех пор, пока не останутся только те разделители, которые не попадают в диапазон

  // находим новую конечную позицию
  auto min_value = input_string.length();
  for (auto& delimiter_position : delimiters_positions) {
    if (delimiter_position.second != std::string::npos) {
      auto position = input_string.find(delimiter_position.first, next_start_position);
      if (position != std::string::npos) {
        if (position < min_value) { 
          min_value = position; 
        }
      }
      delimiter_position.second = position;
    }
  }
  start_position = next_start_position;
  end_position = min_value;

  return *this;
}
