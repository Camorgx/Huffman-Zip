cmake_minimum_required(VERSION 3.20)
project(Huffman_Zip)

set(CMAKE_CXX_STANDARD 14)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
    endif()
endif ()

add_executable(Huffman_Zip main.cpp utils.cpp Tree.cpp Vector.hpp Priority_Queue.hpp)
