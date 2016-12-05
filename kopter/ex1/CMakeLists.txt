cmake_minimum_required(VERSION 3.6)
project(ex1)
find_package(OpenCV REQUIRED)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")
if(CMAKE_VERSION VERSION_LESS "2.8.11")
    # Add OpenCV headers location to your include paths
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

#find_package(OpenGL REQUIRED)
#include_directories(${OpenGL_INCLUDE_DIR})
#set(GLFW_INCLUDE_DIR "/home/moskov/tmp/glfw/include/GLFW/")


#find_package(GLFW REQUIRED)
#include_directories(${GLFW_INCLUDE_DIR})

# Declare the executable target built from your sources
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)

add_executable(ex1 ${SOURCE_FILES})

# Link your application with OpenCV libraries
target_link_libraries(ex1 ${OpenCV_LIBS} )

target_link_libraries(ex1 ${OpenGL_LIBRARIES})

target_link_libraries(ex1 ${GLFW_LIBRARIES} GL GLU glfw GLEW)

