#ifndef HUFFMAN_ZIP_HUFFMAN_HPP
#define HUFFMAN_ZIP_HUFFMAN_HPP

#include <string>
#include "PriorityQueue.hpp"

struct TreeNode {
    int size = 2;
    int deg = 0;
    struct NodeData {
        int word = 0, freq = 0;

        NodeData() = default;
        NodeData(int word, int freq) : word(word), freq(freq) {}
    } data;
    TreeNode** sons = new TreeNode*[2];

    TreeNode() = default;
    explicit TreeNode(const NodeData& data, int size = 2) : size(size), data(data) {
        sons = new TreeNode*[deg];
    }
    TreeNode(int word, int freq, int size = 2) : size(size), data(word, freq) {
        sons = new TreeNode*[deg];
    }
    ~TreeNode() { delete[] sons; }

    bool operator<(const TreeNode& b) const { return data.freq < b.data.freq; }
    bool operator>(const TreeNode& b) const { return data.freq > b.data.freq; }
    bool operator<=(const TreeNode& b) const { return data.freq <= b.data.freq; }
    bool operator>=(const TreeNode& b) const { return data.freq >= b.data.freq; }
};

void display_tree(TreeNode* root, int depth = 0);

void destruct_tree(TreeNode* root);

TreeNode* build_tree(PriorityQueue_Pointers<TreeNode*>& word_freq, int branch = 2);

#endif //HUFFMAN_ZIP_HUFFMAN_HPP
