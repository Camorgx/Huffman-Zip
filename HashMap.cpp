#include "HashMap.hpp"

int int_hash(int key) {
    key = ~key + (key << 15); // key = (key << 15) - key - 1;
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = (key + (key << 3)) + (key << 11);
    key = key ^ (key >> 16);
    return key;
}

HashMap::HashNode* HashMap::insert(int key, int value) {
    int index = int_hash(key) % table_size;
    auto tmp = new HashNode(key, value);
    tmp->next = table[index];
    table[index] = tmp;
    return tmp;
}

void HashMap::del(int key) {
    int index = int_hash(key) % table_size;
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

int* HashMap::find(int key) {
    int index = int_hash(key) % table_size;
    if (!table[index]) return nullptr;
    auto cur = table[index];
    while (cur) {
        if (cur->key == key) return &cur->value;
        else cur = cur->next;
    }
    return nullptr;
}

int& HashMap::operator[](int key) {
    auto ans = find(key);
    return ans ? *ans : insert(key, 0)->value;
}

HashMap::~HashMap() {
    for (const auto i : table) {
        auto tmp = i;
        while (tmp) {
            auto temp = tmp;
            tmp = tmp->next;
            delete temp;
        }
    }
}

HashMap::iterator HashMap::begin() {
    for (int i = 0; i < table_size; ++i)
        if (table[i]) return {i, table[i], table};
    return {-1, nullptr, table};
}

HashMap::iterator HashMap::end() {
    return {-1, nullptr, table};
}

HashMap::iterator& HashMap::iterator::operator++() {
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
