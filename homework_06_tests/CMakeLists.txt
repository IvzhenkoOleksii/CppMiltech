cmake_minimum_required(VERSION 3.20)
project(ballisticCalculations CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/FILES_CPP/*.cpp")

add_library(ballisticLib STATIC
    ${PROJECT_SOURCES}
)

#add_executable(ballisticCalculations 
#    DroneCourseHomework.cpp 
#    ${PROJECT_SOURCES}
#)

target_include_directories(ballisticLib PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/FILES_H"
    "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/NlohmannJson/single_include"
)

target_compile_options(ballisticLib PRIVATE -Wall -Wextra -pedantic)

add_executable(
  ballisticCalculations 
  DroneCourseHomework.cpp 
)
target_link_libraries(
  ballisticCalculations PRIVATE 
  ballisticLib
)

add_executable(
  ballistic_tests
  Tests/ballistic_tests.cpp
)

target_link_libraries(
  ballistic_tests PRIVATE
  ballisticLib
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ballistic_tests)