cmake_minimum_required(VERSION 3.16)
project(SimpC VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/src/header)

# Add subdirectories
add_subdirectory(src)

# Enable testing
# enable_testing()
# add_subdirectory(src/test)

