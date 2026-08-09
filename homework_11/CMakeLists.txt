cmake_minimum_required(VERSION 3.20)
project(executable CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable(executable main.cpp ${PROJECT_SOURCES})
target_link_libraries(executable PRIVATE gpiod)

target_include_directories(
    executable PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/include"
                       "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NlohmannJson/single_include")

target_compile_options(executable PRIVATE -Wall -Wextra -pedantic)
target_compile_definitions(executable PRIVATE PROJECT_ROOT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
