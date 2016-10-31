cmake_minimum_required(VERSION 3.6)
project(algo4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(algo4 ${SOURCE_FILES})
