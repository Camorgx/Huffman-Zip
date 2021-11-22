//
// Created by 曹高翔 on 2021/11/22.
//

#include "utils.hpp"

#include <iostream>
using namespace std;

int main(int argc, const char* argv[]) {
    if (argc == 1) {
        cout << get_help(string(argv[0]));
        return 0;
    }
    for (int i = 1; i < argc; ++i) {

    }
    return 0;
}