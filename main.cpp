#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <mutex>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/post.hpp>
#include <stdexcept>

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

  std::ofstream output_file{ argv[2] };
  if (output_file.is_open()) {
    boost::asio::thread_pool pool{ std::thread::hardware_concurrency() };
    std::mutex write_mutex;
    for (const auto& entry : std::filesystem::directory_iterator{ input_dir_path }) {
      if (entry.is_regular_file()) {
        boost::asio::post(pool, [&output_file, &write_mutex, entry]() {
          try {
            const auto filename = entry.path().filename().string();
            // читаем подстроки из входящего файла во временный вектор
            // чтобы можно было бы это делать в несколько потоков
            std::vector<std::string> substrings{};
            for (auto str: InputContent{ entry.path() }) { 
              substrings.push_back(str);
            }
            // сохраняем подстроки в файл, используя лок на запись,
            // чтобы подстроки одного файла не пересекались с подстроками
            // других файлов
            std::lock_guard write_lock{ write_mutex };
            output_file << "[Имя файла " << filename << "]:"; 
            for (auto str : substrings) {
              output_file << "\n" << str;
            }
            output_file << std::endl;
            std::cout << "Processed file: " << filename << std::endl;
          } catch (std::exception& e) {
            std::cerr << "Error: '" << e.what() << "' while processing " << entry.path().string() << ". Skipping..." << std::endl;
          }
        });
      }
    }
    pool.join();
    output_file.close();
  } else {
    std::cerr << "Error opening output file '" << argv[2] << "'" << std::endl;
  }

  return 0;
}