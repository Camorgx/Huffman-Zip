//
// Created by 曹高翔 on 2021/11/22.
//

#ifndef HUFFMAN_ZIP_UTILS_HPP
#define HUFFMAN_ZIP_UTILS_HPP

#include "Vector.hpp"
#include <string>

void string_split(Vector<std::string>& ans, const std::string& source, const std::string& split = " ");
std::string get_help(const std::string& program_name);

#endif //HUFFMAN_ZIP_UTILS_HPP
