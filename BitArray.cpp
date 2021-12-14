#include "BitArray.hpp"

char BitArray::operator[](int index) {
    if (index >= int(size()))
        throw VectorError("Array out of bounds.");
    int pos = (index + 1) * item_width / 8;
    int e_pos = (index + 1) * item_width % 8;
    if (e_pos == 0) { --pos; e_pos = 8; }
    char ans = 0;
    ans += data[pos] >> (8 - e_pos);
    if (e_pos < item_width) ans += (data[pos - 1] % (1 << (item_width - e_pos))) << e_pos;
    if (e_pos > item_width) ans %= (1 << item_width);
    return ans;
}

size_t BitArray::size() const {
    return ((data.size() - 1) * 8 + bit_left) / item_width;
}

void BitArray::set_value(int index, int value) {
    if (index >= int(size()))
        throw VectorError("Array out of bounds.");
    int pos = (index + 1) * item_width / 8;
    int e_pos = (index + 1) * item_width % 8;
    if (e_pos == 0) { --pos; e_pos = 8; }
    int tmp = data[pos] % (1 << (8 - e_pos)) + ((value % (1 << e_pos)) << (8 - e_pos));
    if (e_pos < item_width)
        data[pos - 1] = ((data[pos - 1] >> (item_width - e_pos)) << (item_width - e_pos))
                + (value >> e_pos);
    if (e_pos > item_width)
        tmp += (data[pos] >> (item_width + 8 - e_pos)) << (item_width + 8 - e_pos);
    data[pos] = tmp;
}

void BitArray::push_back(int value) {
    if (bit_left + item_width <= 8) {
        data[data.size() - 1] += value << (8 - bit_left - item_width);
        bit_left += item_width;
        if (bit_left == 8) {
            data.push_back(0);
            bit_left = 0;
        }
    }
    else {
        data[data.size() - 1] += value >> (bit_left + item_width - 8);
        data.push_back((value % (1 << (7 - bit_left))) << (16 - bit_left - item_width));
        bit_left = bit_left + item_width - 8;
    }
}

void BitArray::pop_back() {
    if (bit_left >= item_width) bit_left -= item_width;
    else {
        bit_left = bit_left + 8 - item_width;
        data.pop_back();
    }
}

bool BitArray::empty() const {
    return data.size() == 1 && bit_left == 0;
}
