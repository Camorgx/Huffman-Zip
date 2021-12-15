#ifndef HUFFMAN_ZIP_HASHMAP_HPP
#define HUFFMAN_ZIP_HASHMAP_HPP

#include "Vector.hpp"

extern int Hash(int key);

template<typename KeyType, typename ValueType>
class HashMap {
    struct HashNode {
        KeyType key{};
        ValueType value{};
        HashNode* next = nullptr;

        HashNode() = default;
        HashNode(KeyType key, ValueType value) : key(key), value(value) {}
    };
    size_t actual_size = 0;
    static const int table_size = 256;
    HashNode* table[table_size] = {nullptr};
public:
    struct iterator {
        int index;
        HashNode* data = nullptr;
        HashNode** table = nullptr;

        iterator(int ind, HashNode* dat, HashNode** tab) : index(ind), data(dat), table(tab) {}

        HashNode& operator*() const { return *data; }
        iterator& operator++() {
            if (data->next) {
                data = data->next;
                return *this;
            }
            for (int i = index + 1; i < table_size; ++i)
                if (table[i]) {
                    index = i; data = table[i];
                    return *this;
                }
            index = -1; data = nullptr;
            return *this;
        }
        bool operator==(const iterator& b) const { return table == b.table && data == b.data; }
        bool operator!=(const iterator& b) const { return !(*this == b); }
    };

    ~HashMap() {
        for (const auto i : table) {
            auto tmp = i;
            while (tmp) {
                auto temp = tmp;
                tmp = tmp->next;
                delete temp;
            }
        }
    }

    HashNode* insert(KeyType key, ValueType value) {
        ++actual_size;
        //The function Hash must have return type int (or unsigned).
        int index = Hash(key) % table_size;
        auto tmp = new HashNode(key, value);
        tmp->next = table[index];
        table[index] = tmp;
        return tmp;
    }
    void del(KeyType key) {
        --actual_size;
        //The function Hash must have return type int (or unsigned).
        int index = Hash(key) % table_size;
        auto cur = table[index];
        if (!cur) return;
        auto pre = cur;
        cur = cur->next;
        while (cur) {
            if (cur->key == key){
                pre->next = cur->next;
                delete cur;
                break;
            }
            else {
                cur = cur->next;
                pre = pre->next;
            }
        }
    }
    ValueType* find(KeyType key) {
        //The function Hash must have return type int (or unsigned).
        int index = Hash(key) % table_size;
        if (!table[index]) return nullptr;
        auto cur = table[index];
        while (cur) {
            if (cur->key == key) return &(cur->value);
            else cur = cur->next;
        }
        return nullptr;
    }
    iterator begin() {
        for (int i = 0; i < table_size; ++i)
            if (table[i]) return {i, table[i], table};
        return {-1, nullptr, table};
    }
    iterator end() { return {-1, nullptr, table}; }
    bool empty() { return begin().data; }
    size_t size() const { return actual_size; }

    ValueType& operator[](KeyType key) {
        auto ans = find(key);
        return ans ? *ans : insert(key, 0)->value;
    }
};


#endif //HUFFMAN_ZIP_HASHMAP_HPP
