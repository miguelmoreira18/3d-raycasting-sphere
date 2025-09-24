cmake_minimum_required(VERSION 3.10)

project(main)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Remove the file(GLOB) command
# file(GLOB SOURCES src/*.cpp)

# List all your source files explicitly
add_executable(main 
    src/main.cpp
    src/Vector3.cpp
)

target_include_directories(main PRIVATE include)