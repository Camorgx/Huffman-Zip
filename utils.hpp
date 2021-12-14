#ifndef HUFFMAN_ZIP_UTILS_HPP
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

//Expand packed files.
void expand_files(const std::string& unzip_path);

//Get data from the input file, store it to an int array and return the array.
Vector<int> prepare_for_zip(const std::string& file_name, unsigned basic_unit_size = 8);

//Get the frequency of each word and insert their opposite number to a heap.
//This is because self-written PriorityQueue is a big-root heap.
PriorityQueue_Pointers<TreeNode*> get_freq(const Vector<int>& data, int branch = 2);

//Get the map relationship of word and its Huffman Code.
HashMap<int, std::string> get_zip_dictionary(PriorityQueue_Pointers<TreeNode*>& word_freq,
                                             bool output_tree = false, int branch = 2);

#endif //HUFFMAN_ZIP_UTILS_HPP
