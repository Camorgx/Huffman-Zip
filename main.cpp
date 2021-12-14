#include "utils.hpp"

#include <cstdio>
#include <iostream>
#include <filesystem>
using namespace std;

enum class Op_Type {
    Zip,
    Unzip,
    Help,
    Unsure
};

//#define TEST

#ifdef TEST
#include "test.hpp"
#endif

int main(int argc, const char* argv[]) {
#ifndef TEST
    Vector<string> input_files;
    string zipped_file_name;
    Op_Type opType = Op_Type::Unsure;
    int size = 8, branch = 2;
    bool no_zip = false;
    bool display_tree = false;
    string current_path = filesystem::current_path().string();
    //Default output path is current path.
    string output_path = current_path;
    bool output_path_set = false;
    if (argc == 1) {
        cout << get_help(string(argv[0]));
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            cerr << "Huffman Zip: Unrecognized option or setting: \"" << argv[i] << '\"' << endl;
            return 1;
        }
        if (argv[i][1] == '-') {
            if (string(argv[i] + 2) == "size") {
                float tmp;
                auto res = sscanf(argv[++i], "%f", &tmp);
                size = int(tmp);
                if (res != 1 || tmp < 0 || float(size) != tmp || size % 4 != 0) {
                    cerr << "Huffman Zip: Invalid size number: \"" << argv[i] << '\"' << endl;
                    return 1;
                }
            }
            else if (string(argv[i] + 2) == "branch") {
                float tmp;
                auto res = sscanf(argv[++i], "%f", &tmp);
                size = int(branch);
                if (res != 1 || tmp < 0 || float(size) != tmp) {
                    cerr << "Huffman Zip: Invalid size number: \"" << argv[i] << '\"' << endl;
                    return 1;
                }
            }
            else if (string(argv[i] + 2) == "display-tree") display_tree = true;
            else if (string(argv[i] + 2) == "no-zip") no_zip = true;
            else {
                cerr << "Huffman Zip: Unrecognized setting: \"" << argv[i] << '\"' << endl;
                return 1;
            }
        }
        else {
            if (string(argv[i] + 1) == "f") {
                if (i == argc - 1) {
                    cerr << "Huffman Zip: You must specify at least one input file after \"-f\"." << endl;
                    return 1;
                }
                while (argv[++i][0] != '-')
                    input_files.push_back(string(argv[i]));
                --i;
            }
            else if (string(argv[i] + 1) == "o") {
                if (i == argc - 1) {
                    cerr << "Huffman Zip: You must specify a filename after \"-o\"." << endl;
                    return 1;
                }
                if (!zipped_file_name.empty()) {
                    cerr << "Huffman Zip: You can only specify the name of output file once." << endl;
                    return 1;
                }
                zipped_file_name = string(argv[++i]);
            }
            else if (string(argv[i] + 1) == "c") {
                if (opType != Op_Type::Unsure) {
                    cerr << "Huffman Zip: You can only specify one mode (-c, -x or -h) at a time." << endl;
                    return 1;
                }
                opType = Op_Type::Zip;
            }
            else if (string(argv[i] + 1) == "x") {
                if (opType != Op_Type::Unsure) {
                    cerr << "Huffman Zip: You can only specify one mode (-c, -x or -h) at a time." << endl;
                    return 1;
                }
                opType = Op_Type::Unzip;
            }
            else if (string(argv[i] + 1) == "h") {
                if (opType != Op_Type::Unsure) {
                    cerr << "Huffman Zip: You can only specify one mode (-c, -x or -h) at a time." << endl;
                    return 1;
                }
                opType = Op_Type::Help;
            }
            else if (string(argv[i] + 1) == "p") {
                if (i == argc - 1) {
                    cerr << "Huffman Zip: You must specify a file name after \"-p\"." << endl;
                    return 1;
                }
                if (output_path_set) {
                    cerr << "Huffman Zip: You can only specify the path of output files once." << endl;
                    return 1;
                }
                output_path_set = true;
                output_path = string(argv[++i]);
            }
            else {
                cerr << "Huffman Zip: Unrecognized option: \"" << argv[i] << '\"' << endl;
                return 1;
            }
        }
    }
    filesystem::path up(output_path);
    if (filesystem::exists(up) && !is_directory(up)) {
        cerr << "Huffman Zip: \"" << output_path << "\" is not a path." << endl;
        return 1;
    }
    if (output_path[output_path.size() - 1] != '/'
        || output_path[output_path.size() - 1] != '\\')
        output_path.append("/");
    if (opType == Op_Type::Help) cout << get_help(string(argv[0]));
    else if (opType == Op_Type::Zip) {
        if (input_files.empty()) {
            cerr << "Huffman Zip: No input files specified." << endl;
            return 1;
        }
        if (zipped_file_name.empty()) zipped_file_name = current_path + "/zip.my_zip";
        if (output_path_set) {
            filesystem::path p(zipped_file_name);
            zipped_file_name = output_path + p.filename().string();
        }
        if (no_zip && !pack_up_files(input_files, zipped_file_name)) return 1;
        if (!pack_up_files(input_files)) return 1;
        auto data = prepare_for_zip("zip_temp.tmp", size);
        auto word_frequency = get_freq(data);
        auto zip_dictionary = get_zip_dictionary(word_frequency, display_tree, branch);
    }
    else if (opType == Op_Type::Unzip) {
        if (input_files.empty()) {
            cerr << "Huffman Zip: No input file specified." << endl;
            return 1;
        }
        if (input_files.size() != 1) {
            cerr << "Huffman Zip: You can only unzip one file at a time." << endl;
            return 1;
        }
        expand_files(output_path);
    }
    else {
        cerr << "Huffman Zip: No operation specified." << endl;
        return 1;
    }
#else
    TestPriorityQueue();
    cout << endl;
    TestHashMap();
#endif
    return 0;
}