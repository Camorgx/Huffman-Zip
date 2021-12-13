#include "utils.hpp"
#include "HashMap.hpp"
#include "Huffman.hpp"
#include <filesystem>
#include <fstream>
#include <cstring>
#include <iostream>
#include <utility>
using namespace std;

void string_split(Vector<string>& ans, const string& source, const string& split) {
    auto pos2 = source.find(split);
    int pos1 = 0;
    while(pos2 != string::npos) {
        ans.push_back(source.substr(pos1, pos2 - pos1));
        pos1 = pos2 + split.size();
        pos2 = source.find(split, pos1);
    }
    if(unsigned(pos1) != source.length())
        ans.push_back(source.substr(pos1));
}

string get_help(const string& program_name) {
    string ans = "Huffman Zip 1.0, using Huffman algorithm to zip files.\nUsage:\n";
    string tmp = program_name, t_tmp;
    for (auto i = tmp.rbegin(); i != tmp.rend(); ++i)
        if (*i != '\\' && *i != '/') t_tmp.push_back(*i);
        else break;
    tmp.clear();
    for (auto i = t_tmp.rbegin(); i != t_tmp.rend(); ++i)
        tmp.push_back(*i);
    ans += tmp + R"( [options] [settings]
options:
    -f Specify input files.
        When you need to zip files, if more than one file is specified, they will be zipped and packed up.
        When you need to unzip files, you can only specify one file.
    -c Zip given files.
    -x Unzip given file.
    -h Show this help.
    -o Specify the name of zipped file. This option will be effective only if "-c" is specified.
    -p Specify the output path. If the path doesn't exist, the program will create it.
settings:
    --size Specify basic unit size used in zip progress. This option will be effective only if "-c" is specified.
        The number following "--size" should be an positive integer, count by bit. It should be multiple of 4 and between 4 and 32.
    --branch Specify the number of branches of Huffman tree used in zip progress. This option will be effective only if "-c" is specified.
        The number following "--branch" should be an positive integer between 2 and 127.
    --display-tree Display the Huffman tree used to zip files. This option will be effective only if "-c" is specified.
        The tree will be displayed in the form of an embedded table.
)";
    return ans;
}

Vector<int> prepare_for_zip(const std::string& file_name, const unsigned basic_unit_size) {
    filesystem::path file_path(file_name);
    auto f_size = file_size(file_path);
    ifstream fin(file_name, ios::binary | ios::in);
    Vector<int> ans, tmp;
    if (!fin) return ans;
    char* buf = new char[f_size];
    fin.read(buf, f_size);
    for (unsigned long i = 0; i < f_size; ++i) {
        tmp.push_back(int(buf[i] >> 4));
        tmp.push_back(int(buf[i] & 0xf));
    }
    if (!(ans.size() % (basic_unit_size / 4))) {
        // Count by the number of 4-bit segments.
        int append_size = (2 * f_size / (basic_unit_size / 4) + 1) - 2 * f_size;
        for (int i = 0; i < append_size; ++i)
            tmp.push_back(0);
    }
    for (size_t i = 0; i < tmp.size();) {
        int t_tmp = 0;
        for (int j = basic_unit_size / 4 - 1; j >= 0; --j) {
            t_tmp += tmp[i++] << (j * 4);
        }
        ans.push_back(t_tmp);
    }
    delete[] buf;
    return ans;
}

PriorityQueue_Pointers<TreeNode*> get_freq(const Vector<int> &data, int branch) {
    HashMap<int, int> map;
    PriorityQueue_Pointers<TreeNode*> ans;
    for (const auto &item: data) ++map[item];
    for (const auto &item: map)
        ans.push(new TreeNode(item.key, -item.value, branch));
    return ans;
}

bool pack_up_files(const Vector<std::string>& input_files) {
    for (const auto& item : input_files) {
        try {
            filesystem::path file_path(item);
            filesystem::file_size(file_path);
        }
        catch (const filesystem::filesystem_error& e) {
            auto err = string(e.what());
            cerr << "Huffman Zip: " << err.substr(err.find("file_size:") + 11) << endl;
            return false;
        }
    }
    ofstream output("zip_temp.tmp", ios::out | ios::binary);
    short num_of_input = input_files.size();
    //Write num of files.
    output.write((char*)(&num_of_input), sizeof(num_of_input));
    for (const auto& item : input_files) {
        filesystem::path file_path(item);
        auto file_name = (const char*)filesystem::relative(file_path).c_str();
        short length = strlen(file_name);
        ifstream fin(item, ios::in | ios::binary);
        int file_size = filesystem::file_size(file_path);
        char* content = new char[file_size];
        fin.read(content, file_size);
        //Write length of filename.
        output.write((char*)(&length), sizeof(length));
        //Write filename.
        output.write(file_name, length * sizeof(char));
        //Write size of the file.
        output.write((char*)(&file_size), sizeof(file_size));
        //Write the content of the file.
        output.write(content, file_size * sizeof(char));
        fin.close();
    }
    output.close();
    return true;
}

void expand_files(const string& unzip_path) {
    ifstream input("zip_temp.tmp", ios::in | ios::binary);
    filesystem::path p(unzip_path);
    if (!filesystem::exists(p))
        filesystem::create_directory(p);
    short num_of_input;
    //Read num of files.
    input.read((char*)(&num_of_input), sizeof(num_of_input));
    for (int i = 0; i < num_of_input; ++i) {
        //Read length of filename.
        short length_of_name;
        input.read((char*)(&length_of_name), sizeof(length_of_name));
        //Read filename.
        char* filename = new char[length_of_name + 1];
        input.read(filename, length_of_name * sizeof(char));
        filename[length_of_name] = '\0';
        //Read size of the file.
        int file_size;
        input.read((char*)(&file_size), sizeof(file_size));
        //Read the content of the file.
        char* content = new char[file_size];
        input.read(content, file_size * sizeof(char));
        ofstream output(unzip_path + string(filename), ios::out | ios::binary);
        output.write(content, file_size * sizeof(char));
        output.close();
    }
    input.close();
}

int Hash(int key) {
    key = ~key + (key << 15); // key = (key << 15) - key - 1;
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = (key + (key << 3)) + (key << 11);
    key = key ^ (key >> 16);
    return key;
}

void get_huffman_code(TreeNode* root, HashMap<int, string>& ans, const string& current = "") {
    if (!root) return;
    if (root->deg == 0) {
        ans.insert(root->data.word, current);
        return;
    }
    for (int i = 0; i < root->deg; ++i)
        get_huffman_code(root->sons[i], ans, current + char(i));
}

HashMap<int, string> get_zip_dictionary(
        PriorityQueue_Pointers<TreeNode*>& word_freq, bool output_tree, int branch) {
    auto tree = build_tree(word_freq, branch);
    if (output_tree) display_tree(tree);
    HashMap<int, string> ans;
    get_huffman_code(tree, ans);
    destruct_tree(tree);
    return ans;
}
