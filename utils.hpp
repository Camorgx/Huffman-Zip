#ifndef HUFFMAN_ZIP_UTILS_HPP
#define HUFFMAN_ZIP_UTILS_HPP

#include "Vector.hpp"
#include "Huffman.hpp"
#include <string>

void string_split(Vector<std::string>& ans, const std::string& source, const std::string& split = " ");
std::string get_help(const std::string& program_name);
Vector<int> prepare_for_zip(const std::string& file_name, const unsigned basic_unit_size = 8);

#endif //HUFFMAN_ZIP_UTILS_HPP
