cmake_minimum_required(VERSION 3.20)
project(CppMiltech LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Домашні роботи. По мірі появи додаємо кожну окремим add_subdirectory.
add_subdirectory(DroneCourseHomework)
add_subdirectory(homework_04_odometry)