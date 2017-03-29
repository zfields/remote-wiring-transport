cmake_minimum_required(VERSION 2.8.2)
project(SERIAL_WIRING)

set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Wextra -Werror")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_subdirectory(src)
add_subdirectory(include)
add_subdirectory(samples)
