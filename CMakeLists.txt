cmake_minimum_required(VERSION 3.20)
project(Huffman_Zip)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
elseif (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif ()

include_directories(Huffman_Zip include/)

add_executable(Huffman_Zip src/main.cpp src/utils.cpp src/Huffman.cpp src/BitArray.cpp src/Hash.cpp)
