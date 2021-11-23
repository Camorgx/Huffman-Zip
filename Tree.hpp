//
// Created by 曹高翔 on 2021/11/23.
//

#ifndef HUFFMAN_ZIP_TREE_HPP
#define HUFFMAN_ZIP_TREE_HPP

struct TreeNode {
    int dat = 0, deg = 2;
    TreeNode** sons = new TreeNode*[2];

     ~TreeNode() { delete[] sons; }
};

class Tree {
    void destruct_tree(TreeNode* node);
public:
    TreeNode* root = nullptr;

    ~Tree();
};

#endif //HUFFMAN_ZIP_TREE_HPP
