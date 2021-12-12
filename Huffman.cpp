#include "Huffman.hpp"
#include <iostream>
using namespace std;

void display_tree(TreeNode* root, int depth) {
    if (!root) return;
    for(int i = 0; i < depth; ++i) cout << '\t';
    cout << root->data.word << endl;
    for (int i = 0; i < root->deg; ++i)
        display_tree(root->sons[i], depth + 1);
}