cmake_minimum_required(VERSION 3.20)
project(Huffman_Zip)

set(CMAKE_CXX_STANDARD 17)

add_executable(Huffman_Zip main.cpp utils.cpp Tree.cpp Tree.hpp)
