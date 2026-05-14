cmake_minimum_required(VERSION 3.20)
project(debugging CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(debugging 
    src/main.cpp 
    src/telemetry.cpp
)

target_include_directories(debugging PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_compile_options(debugging PRIVATE -Wall -Wextra -pedantic)