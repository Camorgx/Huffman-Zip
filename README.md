# Huffman Zip

This is a program written in C++ to zip or unzip files or just pack them up.

The program mainly uses Huffman algorithm to do the job. In the application, 
you can specify the size of basic units of the zip dictionary or the num of branches
of the Huffman tree. What's more, you can request the application to 
display the Huffman tree in the form of an embedded table if you want to do so.

To build and use it, you will need CMake and a compiler which support C++17.

## Build

It is simple to build the program.

```shell
mkdir "Huffman Zip" && cd "Huffman Zip"
git clone https://git.ustc.edu.cn/bill/huffman-zip.git ./
mkdir build && cd build
cmake ../
make
```

## Usage

Type Huffman_Zip to get detailed usage and the information of each option/setting.
