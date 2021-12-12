#ifndef HUFFMAN_ZIP_HUFFMAN_HPP
#define HUFFMAN_ZIP_HUFFMAN_HPP

#include <string>

struct TreeNode {
    int deg = 2, content = 0;
    std::string code;
    TreeNode** sons = new TreeNode*[2];

    ~TreeNode() { delete[] sons; }
};

void display_tree(TreeNode* root, int depth = 0);

#endif //HUFFMAN_ZIP_HUFFMAN_HPP
