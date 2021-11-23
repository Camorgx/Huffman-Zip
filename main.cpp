//
// Created by 曹高翔 on 2021/11/22.
//

#include "utils.hpp"

#include <cstdio>
#include <iostream>
using namespace std;

enum class Op_Type {
    Zip,
    Unzip,
    Help,
    Unsure
};

int main(int argc, const char* argv[]) {
    vector<string> input_files;
    string output_file;
    Op_Type opType = Op_Type::Unsure;
    int size = 8, branch = 2;
    if (argc == 1) {
        cout << get_help(string(argv[0]));
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            cout << "Huffman zip: Unrecognized option or setting: \"" << argv[i] << '\"' << endl;
            return 1;
        }
        if (argv[i][1] == '-') {
            if (string(argv[i] + 2) == "size") {
                float tmp;
                auto res = sscanf(argv[i] + 2, "%f", &tmp);
                size = int(tmp);
                if (res != 1 || tmp < 0 || float(size) != tmp) {
                    cout << "Huffman zip: Invalid size number: \"" << argv[i] << '\"' << endl;
                    return 1;
                }
            }
            else if (string(argv[i] + 2) == "branch") {
                float tmp;
                auto res = sscanf(argv[i] + 2, "%f", &tmp);
                size = int(branch);
                if (res != 1 || tmp < 0 || float(size) != tmp) {
                    cout << "Huffman zip: Invalid size number: \"" << argv[i] << '\"' << endl;
                    return 1;
                }
            }
            else {
                cout << "Huffman zip: Unrecognized setting: \"" << argv[i] << '\"' << endl;
                return 1;
            }
        }
        else {
            if (string(argv[i] + 1) == "f") {
                if (i == argc - 1) {
                    cout << "Huffman zip: You must specify at least one input file after \"-f\"." << endl;
                    return 1;
                }
                while (argv[++i][0] != '-')
                    input_files.push_back(string(argv[i]));
                --i;
            }
            else if (string(argv[i] + 1) == "o") {
                if (i == argc - 1) {
                    cout << "Huffman zip: You must specify a input_files after \"-o\"." << endl;
                    return 1;
                }
                if (!output_file.empty()) {
                    cout << "Huffman zip: You can only specify the name of output file once." << endl;
                    return 1;
                }
                output_file = string(argv[++i]);
            }
            else if (string(argv[i] + 1) == "c") {
                if (opType != Op_Type::Unsure) {
                    cout << "Huffman zip: You can only specify one mode (-c, -x or -h) at a time." << endl;
                    return 1;
                }
                opType = Op_Type::Zip;
            }
            else if (string(argv[i] + 1) == "x") {
                if (opType != Op_Type::Unsure) {
                    cout << "Huffman zip: You can only specify one mode (-c, -x or -h) at a time." << endl;
                    return 1;
                }
                opType = Op_Type::Unzip;
            }
            else if (string(argv[i] + 1) == "h") {
                if (opType != Op_Type::Unsure) {
                    cout << "Huffman zip: You can only specify one mode (-c, -x or -h) at a time." << endl;
                    return 1;
                }
                opType = Op_Type::Help;
            }
            else {
                cout << "Huffman zip: Unrecognized option: \"" << argv[i] << '\"' << endl;
                return 1;
            }
        }
    }
    switch (opType) {
        case Op_Type::Help:
            cout << get_help(string(argv[0]));
            break;
        case Op_Type::Zip:
            if (input_files.empty()) {
                cout << "Huffman zip: No input_files specified." << endl;
                return 1;
            }
            break;
        case Op_Type::Unzip:
            if (input_files.empty()) {
                cout << "Huffman zip: No input_files specified." << endl;
                return 1;
            }
            if (input_files.size() != 1) {
                cout << "Huffman zip: You can only unzip one file at a time." << endl;
                return 1;
            }
            break;
        default:
            cout << "Huffman zip: No operation specified." << endl;
            return 1;
    }
    cout << "Input files: " << endl;
    for (const auto& i : input_files) {
        cout << i << endl;
    }
    cout << "Output file: " << output_file << endl;
    return 0;
}