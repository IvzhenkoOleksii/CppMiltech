cmake_minimum_required(VERSION 3.20)
project(ugv_odometry CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/FILES_CPP/*.cpp")


add_executable(ugv_odometry 
    src/main.cpp 
    ${PROJECT_SOURCES}
)

target_include_directories(ugv_odometry PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/FILES_H"
)

target_compile_options(ugv_odometry PRIVATE -Wall -Wextra -pedantic)