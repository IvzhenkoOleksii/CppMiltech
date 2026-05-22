cmake_minimum_required(VERSION 3.20)
add_compile_options(-Wall -Wextra -pedantic)

project(debugging CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(telemetryLib STATIC src/telemetry.cpp)
target_compile_options(telemetryLib PRIVATE)

target_include_directories(
    telemetryLib
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_executable(debugging src/telemetry_check.cpp)
target_link_libraries(debugging PRIVATE telemetryLib)
target_compile_options(debugging PRIVATE)
