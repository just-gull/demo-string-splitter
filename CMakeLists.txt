cmake_minimum_required(VERSION 3.10)

project(demo-splitter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(THREADS_PREFER_PTHREAD_FLAG True)

find_package(Boost 1.81.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  
  find_package(Threads REQUIRED)
  add_executable(demo-splitter input-content.cpp main.cpp)
  target_link_libraries (demo-splitter Threads::Threads)
endif()
