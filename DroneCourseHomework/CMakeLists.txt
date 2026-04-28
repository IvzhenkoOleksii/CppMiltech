cmake_minimum_required(VERSION 3.20)
project(droneSeekTarget CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(droneSeekTarget DroneCourseHomework.cpp)
target_include_directories(
    droneSeekTarget 
    PRIVATE 
    ThirdParty/NlohmannJson/single_include)
target_include_directories(
    droneSeekTarget 
    PRIVATE 
    FILES_H)
target_include_directories(
    droneSeekTarget 
    PRIVATE 
    FILES_CPP)



target_compile_options(droneSeekTarget PRIVATE -Wall -Wextra -pedantic)