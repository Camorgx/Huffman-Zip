#include "Hash.hpp"
using namespace std;

int Hash(int key) {
    key = ~key + (key << 15); // key = (key << 15) - key - 1;
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = (key + (key << 3)) + (key << 11);
    key = key ^ (key >> 16);
    return key;
}

unsigned Hash(const string& key) {
    unsigned ans = 0;
    for (unsigned i = 0; i < key.size(); ++i)
        ans = (ans * 131 + key[i]) % 19491001;
    return ans;
}
