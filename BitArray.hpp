#ifndef HUFFMAN_ZIP_BITARRAY_HPP
#define HUFFMAN_ZIP_BITARRAY_HPP

#include "Vector.hpp"

//Item width must be smaller than 8.
class BitArray {
    Vector<unsigned char> data = {0};
    const int item_width;
public:
    //Num of bits left after the last complete char.
    int bit_left = 0;
    BitArray() : item_width(1) {}
    explicit BitArray(int width) : item_width(width) {
        if (width >= 8)
            throw std::runtime_error("BitArray: Item width is too large.");
    }
    BitArray(int width, unsigned char* data, unsigned size, int bit_left)
        : data(data, size), item_width(width), bit_left(bit_left) {}

    char operator[](int index) const;

    size_t size() const;
    bool empty() const;
    void set_value(int index, unsigned value);
    void push_back(unsigned value);
    void pop_back();
    const auto& get_data() const { return data; }
};


#endif //HUFFMAN_ZIP_BITARRAY_HPP
