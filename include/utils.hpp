﻿#ifndef HUFFMAN_ZIP_UTILS_HPP
#define HUFFMAN_ZIP_UTILS_HPP

#include "Vector.hpp"
#include "PriorityQueue.hpp"
#include "HashMap.hpp"
#include "Huffman.hpp"
#include <string>

void string_split(Vector<std::string>& ans, const std::string& source, const std::string& split = " ");

//Return help document of the program.
std::string get_help(const std::string& program_name);

//Pack up files to a single file.
bool pack_up_files(const Vector<std::string>& input_file, const std::string& filename = "zip_temp.tmp");

//Get data from the input file, store it to an int array and return the array.
Vector<unsigned> prepare_for_zip(const std::string& file_name, unsigned basic_unit_size, int& append_size);

//Get the frequency of each word and insert their opposite number to a heap.
//This is because self-written PriorityQueue is a big-root heap.
PriorityQueue_Pointers<TreeNode*> get_freq(const Vector<unsigned>& data, int branch = 2);

//Get the map relationship of word and its Huffman Code.
HashMap<unsigned, std::string> get_zip_dictionary(PriorityQueue_Pointers<TreeNode*>& word_freq,
                                             bool output_tree = false, int branch = 2);

bool zip_files(const std::string& output_file, HashMap<unsigned, std::string>& dict,
               const Vector<unsigned>& data, int branch = 2, short basic_unit_size = 8, int append_size = 0);

bool unzip_file(const std::string& input_file, const std::string& output_path);

#endif //HUFFMAN_ZIP_UTILS_HPP
