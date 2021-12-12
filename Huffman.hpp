#ifndef HUFFMAN_ZIP_HUFFMAN_HPP
#define HUFFMAN_ZIP_HUFFMAN_HPP

#include <string>

struct TreeNode {
    int deg = 2;
    struct NodeData {
        int word = 0, freq = 0;

        NodeData() = default;
        NodeData(int word, int freq) : word(word), freq(freq) {}

        bool operator<(const NodeData& b) const { return freq < b.freq; }
        bool operator>(const NodeData& b) const { return freq > b.freq; }
        bool operator<=(const NodeData& b) const { return freq <= b.freq; }
        bool operator>=(const NodeData& b) const { return freq >= b.freq; }
    } data;
    TreeNode** sons = new TreeNode*[2];

    TreeNode() = default;
    explicit TreeNode(const NodeData& data, int deg = 2) : deg(deg), data(data) {
        sons = new TreeNode*[deg];
    }
    TreeNode(int word, int freq, int deg = 2) : deg(deg), data(word, freq) {
        sons = new TreeNode*[deg];
    }
    ~TreeNode() { delete[] sons; }
};

void display_tree(TreeNode* root, int depth = 0);

#endif //HUFFMAN_ZIP_HUFFMAN_HPP
