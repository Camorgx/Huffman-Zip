#ifndef HUFFMAN_ZIP_HASH_HPP
#define HUFFMAN_ZIP_HASH_HPP

#include <string>
#include "Vector.hpp"

int Hash(int key);

unsigned Hash(const std::string& key);

template<typename DataType>
unsigned Hash(const Vector<DataType>& key) {
    unsigned ans = 0;
    for (unsigned i = 0; i < key.size(); ++i)
        ans = (ans * 131 + key[i]) % 19491001;
    return ans;
}

#endif //HUFFMAN_ZIP_HASH_HPP
