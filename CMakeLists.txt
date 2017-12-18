cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(EABC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Os -Wall -pedantic")

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("googletest")

