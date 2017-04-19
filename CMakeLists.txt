cmake_minimum_required(VERSION 2.8.2)
cmake_policy(SET CMP0048 NEW)
project(SERIAL_WIRING VERSION 0.1.2 LANGUAGES CXX)

# Compile settings
option(LOG "Enable debug log" OFF)
if (LOG)
  message(STATUS "Debug logging enabled...")
  add_definitions(-DLOG)
endif()

# Prerequisites
find_package(Git REQUIRED)

# serial_wiring
add_library(serial_wiring ${CMAKE_CURRENT_SOURCE_DIR}/src/UartSerial.cpp)

target_include_directories(serial_wiring
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/serial-wiring>
)

set_target_properties(serial_wiring
  PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY lib
    LIBRARY_OUTPUT_DIRECTORY lib
)

# To view the list of supported compiler features, see
# (https://cmake.org/cmake/help/latest/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html)
target_compile_features(serial_wiring
  PUBLIC
    cxx_nullptr
    cxx_override
    cxx_generalized_initializers
)

target_compile_options(serial_wiring
  PUBLIC -Wall -Wextra -Werror
)

# Build sample applications
add_subdirectory(samples)
