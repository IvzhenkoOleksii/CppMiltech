cmake_minimum_required(VERSION 3.20)
project(droneSeekTarget CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


file(GLOB PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/FILES_CPP/*.cpp")

add_executable(droneSeekTarget 
    DroneCourseHomework.cpp 
    ${PROJECT_SOURCES}
)

target_include_directories(droneSeekTarget PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/FILES_H"
    "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NlohmannJson/single_include"
)


target_compile_options(droneSeekTarget PRIVATE -Wall -Wextra -pedantic)