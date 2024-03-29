﻿#include "Huffman.hpp"
#include <iostream>
using namespace std;

void display_tree(TreeNode* root, int depth) {
    if (!root) return;
    for(int i = 0; i < depth; ++i) cout << '\t';
    cout << "(" << root->data.word << ", " << root->data.freq << ")\n";
    for (int i = 0; i < root->size; ++i)
        display_tree(root->sons[i], depth + 1);
}

TreeNode* build_tree(PriorityQueue_Pointers<TreeNode*>& word_freq, int branch) {
    while (word_freq.size() >= branch) {
        auto* root = new TreeNode(-1, 0, branch);
        for (int i = 0; i < branch; ++i) {
            auto data = word_freq.top(); word_freq.pop();
            root->sons[root->deg++] = data;
            root->data.freq += data->data.freq;
        }
        word_freq.push(root);
    }
    if (word_freq.size() == 1) return word_freq.top();
    auto* root = new TreeNode(-1, 0, branch);
    while (!word_freq.empty()) {
        auto data = word_freq.top(); word_freq.pop();
        root->sons[root->deg++] = data;
    }
    return root;
}

void destruct_tree(TreeNode* root) {
    if (!root) return;
    for (int i = 0; i < root->deg; ++i)
        destruct_tree(root->sons[i]);
    delete root;
}
