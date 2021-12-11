#ifndef HUFFMAN_ZIP_HUFFMAN_HPP
#define HUFFMAN_ZIP_HUFFMAN_HPP

struct TreeNode {
    int deg = 2;
    TreeNode** sons = new TreeNode*[2];

     ~TreeNode() { delete[] sons; }
};

struct MinUnit {
    unsigned short dat : 4;

    MinUnit() = default;
    MinUnit(short data) : dat(data) {}
};

#endif //HUFFMAN_ZIP_HUFFMAN_HPP
