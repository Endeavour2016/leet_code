cmake_minimum_required(VERSION 3.20)

project(leetcode_practice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CTest)
include(cmake/AddLeetCodeProblem.cmake)

add_subdirectory(algo)

add_subdirectory(engineering)
