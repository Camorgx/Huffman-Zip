#ifndef HUFFMAN_ZIP_UTILS_HPP
#define HUFFMAN_ZIP_UTILS_HPP

#include "Vector.hpp"
#include "PriorityQueue.hpp"
#include "Huffman.hpp"
#include <string>

void string_split(Vector<std::string>& ans, const std::string& source, const std::string& split = " ");
std::string get_help(const std::string& program_name);

//Pack up files to a single file with the name "zip_temp.tmp"
bool pack_up_files(const Vector<std::string>& input_files);
//Expand packed files.
void expand_files(const std::string& unzip_path);
//Get data from the input file, store it to an int array and return the array.
Vector<int> prepare_for_zip(const std::string& file_name, unsigned basic_unit_size = 8);
//Get the frequency of each word and insert their opposite number to a heap.
//This is because self-written PriorityQueue is a big-root heap.
PriorityQueue<TreeNode::NodeData> get_freq(const Vector<int>& data);

#endif //HUFFMAN_ZIP_UTILS_HPP
