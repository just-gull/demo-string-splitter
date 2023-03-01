cmake_minimum_required(VERSION 3.10)

project(demo-splitter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost 1.81.0)
if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(demo-splitter input-content.cpp main.cpp)
endif()
