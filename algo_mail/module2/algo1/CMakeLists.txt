cmake_minimum_required(VERSION 3.6)
project(algo1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(algo1 ${SOURCE_FILES})