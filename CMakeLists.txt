cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(EABC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Debug)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("fmtlib")
add_subdirectory("googletest")
add_subdirectory("src")

