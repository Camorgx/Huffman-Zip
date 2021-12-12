#ifndef HUFFMAN_ZIP_HASHMAP_HPP
#define HUFFMAN_ZIP_HASHMAP_HPP

#include "Vector.hpp"

class HashMap {
    struct HashNode {
        int key = 0, value = 0;
        HashNode* next = nullptr;

        HashNode() = default;
        HashNode(int key, int value) : key(key), value(value) {}
    };
    static const int table_size = 256;
    HashNode* table[table_size] = {nullptr};
public:
    struct iterator {
        int index;
        HashNode* data = nullptr;
        HashNode** table = nullptr;

        iterator(int ind, HashNode* dat, HashNode** tab) : index(ind), data(dat), table(tab) {}

        HashNode& operator*() const { return *data; }
        iterator& operator++();
        bool operator==(const iterator& b) const { return table == b.table && data == b.data; }
        bool operator!=(const iterator& b) const { return !(*this == b); }
    };

    ~HashMap();

    HashNode* insert(int key, int value);
    void del(int key);
    int* find(int key);
    iterator begin();
    iterator end();
    bool empty() { return begin().data; }

    int& operator[](int key);
};


#endif //HUFFMAN_ZIP_HASHMAP_HPP
