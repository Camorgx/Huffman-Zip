#include "utils.hpp"
#include "HashMap.hpp"
#include "Huffman.hpp"
#include "BitArray.hpp"
#include <filesystem>
#include <fstream>
#include <cstring>
#include <iostream>
#include <utility>
#include <cmath>
using namespace std;

//#define DEBUG_UTILS

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
    -o Specify the name of zipped file.
        This option will be effective only if "-c" is specified. You can also specify output path without "-p".
    -p Specify the output path.
        If the path doesn't exist, the program will create it. Default output path is current path.
        If "-o" also specified output path, it will be overwritten.
settings:
    --size Specify basic unit size used in zip progress. This option will be effective only if "-c" is specified.
        The number following "--size" should be an positive integer, count by bit. It should be multiple of 4 and between 4 and 32.
    --branch Specify the number of branches of Huffman tree used in zip progress. This option will be effective only if "-c" is specified.
        The number following "--branch" should be an positive integer between 2 and 127.
    --display-tree Display the Huffman tree used to zip files. This option will be effective only if "-c" is specified.
        The tree will be displayed in the form of an embedded table.
    --no-zip If specified, the program will only pack the files up. This setting will disable "--display-tree".
)";
    return ans;
}

Vector<unsigned> prepare_for_zip(const string& file_name, const unsigned basic_unit_size,
                            int& append_size) {
    filesystem::path file_path(file_name);
    auto f_size = file_size(file_path);
    ifstream fin(file_name, ios::binary | ios::in);
    Vector<unsigned> ans, tmp;
    if (!fin) return ans;
    char* buf = new char[f_size];
    fin.read(buf, f_size);
    for (unsigned long i = 0; i < f_size; ++i) {
        tmp.push_back(unsigned(buf[i] >> 4));
        tmp.push_back(unsigned(buf[i] & 0xf));
    }
    if ((tmp.size() % (basic_unit_size / 4))) {
        // Count by the number of 4-bit segments.
        append_size = ((2 * f_size / (basic_unit_size / 4)) + 1) * (basic_unit_size / 4) - 2 * f_size;
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
    fin.close();
#ifdef DEBUG_UTILS
    for (const auto& i : ans)
        cout << i << ' ';
    cout << endl;
#endif //DEBUG_UTILS
    return ans;
}

PriorityQueue_Pointers<TreeNode*> get_freq(const Vector<unsigned> &data, int branch) {
    HashMap<unsigned, int> map;
    PriorityQueue_Pointers<TreeNode*> ans;
    for (const auto &item: data) ++map[item];
    for (const auto &item: map) {
        ans.push(new TreeNode(item.key, -item.value, branch));
#ifdef DEBUG_UTILS
        cout << item.key << ' ' << item.value << endl;
#endif //DEBUG_UTILS
    }
    return ans;
}

bool pack_up_files(const Vector<string>& input_files, const string& filename) {
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
    ofstream output(filename, ios::out | ios::binary);
    //Write flag.
    unsigned char not_zipped = 0;
    output.write((char*)(&not_zipped), sizeof(not_zipped));
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

void expand_files(const std::string& input_file, const string& unzip_path) {
    ifstream input(input_file, ios::in | ios::binary);
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

void get_huffman_code(TreeNode* root, HashMap<unsigned, string>& ans, const string& current = "") {
    if (!root) return;
    if (root->deg == 0) {
        ans.insert(root->data.word, current);
        return;
    }
    for (int i = 0; i < root->deg; ++i)
        get_huffman_code(root->sons[i], ans, current + char(i));
}

HashMap<unsigned, string> get_zip_dictionary(
        PriorityQueue_Pointers<TreeNode*>& word_freq, bool output_tree, int branch) {
    auto tree = build_tree(word_freq, branch);
    if (output_tree) display_tree(tree);
    HashMap<unsigned, string> ans;
    get_huffman_code(tree, ans);
    destruct_tree(tree);
#ifdef DEBUG_UTILS
    for (const auto& i : ans) {
        cout << i.key << ' ';
        for (const auto& j : i.value)
            cout << int(j);
        cout << endl;
    }
#endif //DEBUG_UTILS
    return ans;
}

bool zip_files(const string& output_file, HashMap<unsigned, string>& dict,
               const Vector<unsigned>& data, int branch, short basic_unit_size, int append_size) {
    filesystem::path p(output_file);
    if (filesystem::exists(p) && is_directory(p)) {
        cerr << "Huffman Zip: \"" << output_file << "\" is a directory." << endl;
        return false;
    }
    auto parent_path = p.parent_path();
    if (!parent_path.string().empty() && !filesystem::exists(parent_path))
        filesystem::create_directory(parent_path);
    ofstream output(output_file, ios::out | ios::binary);
    //Write flag.
    unsigned char zipped = 1;
    output.write((char*)(&zipped), sizeof(zipped));
    //Write basic unit size.
    output.write((char*)(&basic_unit_size), sizeof(basic_unit_size));
    //Write code bit-width.
    auto code_bit_width = short(ceil(log2(branch)));
    output.write((char*)(&code_bit_width), sizeof(code_bit_width));
    //Write append size.
    output.write((char*)(&append_size), sizeof(append_size));
    //Write size of the zip dictionary.
    unsigned dict_size = dict.size();
    output.write((char*)(&dict_size), sizeof(dict_size));
    //Write the zip dictionary.
    for (const auto& word : dict) {
        BitArray code(code_bit_width);
        for (const auto& item : word.value) code.push_back(item);
        auto code_out = code.get_data();
        //Write the size of the BitArray of the code.
        unsigned size = code_out.size();
        output.write((char*)(&size), sizeof(size));
        //Write bit_left of the BitArray.
        output.write((char*)(&code.bit_left), sizeof(code.bit_left));
        //Write the code.
        output.write((char*)code_out.c_array(), code_out.size() * sizeof(char));
        BitArray ori_word(4);
        for (int i = basic_unit_size / 4 - 1; i >= 0; --i) {
            unsigned tmp = (word.key >> (i * 4)) - ((word.key >> ((i + 1) * 4)) << ((i + 1) * 4));
            ori_word.push_back(tmp);
        }
        const auto& out_word = ori_word.get_data();
        //Write the size of the BitArray of the word.
        unsigned word_size = out_word.size();
        output.write((char*)(&word_size), sizeof(word_size));
        //Write bit_left of the BitArray.
        output.write((char*)(&ori_word.bit_left), sizeof(ori_word.bit_left));
        //Write the word.
        output.write((char*)out_word.c_array(), out_word.size() * sizeof(char));
    }
    BitArray out(code_bit_width);
    for (const auto& word : data) {
        const auto& code = dict[word];
        for (const auto& item : code) out.push_back(item);
    }
    const auto& out_file = out.get_data();
    //Write the size of the BitArray of the zipped file.
    auto size = out_file.size();
    output.write((char*)(&size), sizeof(size));
    //Write bit_left of the BitArray.
    output.write((char*)(&out.bit_left), sizeof(out.bit_left));
    //Write the zipped file.
    output.write((char*)out_file.c_array(), output_file.size() * sizeof(char));
    output.close();
    return true;
}

bool unzip_file(const string& input_file, const string& output_path) {
    try {
        filesystem::path file_path(input_file);
        filesystem::file_size(file_path);
    }
    catch (const filesystem::filesystem_error& e) {
        auto err = string(e.what());
        cerr << "Huffman Zip: " << err.substr(err.find("file_size:") + 11) << endl;
        return false;
    }
    ifstream input(input_file, ios::in | ios::binary);
    //Read flag.
    unsigned char flag;
    input.read((char*)(&flag), sizeof(flag));
    if (flag == 0) {
        expand_files(input_file, output_path);
        return true;
    }
    //Read basic unit size.
    short basic_unit_size;
    input.read((char*)(&basic_unit_size), sizeof(basic_unit_size));
    //Read code bit-width.
    short code_bit_width;
    input.read((char*)(&code_bit_width), sizeof(code_bit_width));
    //Read append size.
    int append_size;
    input.read((char*)(&append_size), sizeof(append_size));
    //Read size of the zip dictionary.
    unsigned dict_size;
    input.read((char*)(&dict_size), sizeof(dict_size));
    //Read the zip dictionary.
    HashMap<string, unsigned> anti_dict;
    for (unsigned i = 0; i < dict_size; ++i) {
        //Read the size of the BitArray of the code.
        unsigned code_size;
        input.read((char*)(&code_size), sizeof(code_size));
        //Read bit_left of the BitArray.
        int code_bit_left;
        input.read((char*)(&code_bit_left), sizeof(code_bit_left));
        //Read the code.
        auto* code = new unsigned char[code_size];
        input.read((char*)code, code_size * sizeof(char));
        //Read the size of the BitArray of the word.
        unsigned word_size;
        input.read((char*)(&word_size), sizeof(word_size));
        //Read bit_left of the BitArray.
        int word_bit_left;
        input.read((char*)(&word_bit_left), sizeof(word_bit_left));
        //Read the code.
        auto* word = new unsigned char[code_size];
        input.read((char*)word, word_size * sizeof(char));
        BitArray code_array(code_bit_width, code, code_size, code_bit_left),
                word_array(4, word, word_size, word_bit_left);
        string ins_code;
        for (unsigned j = 0; j < code_size; ++j) ins_code.push_back(code_array[j]);
        unsigned ins_word = 0;
        for (unsigned j = word_size - 1; j >= 0; --j)
            ins_word += (word_array[j] << (4 * j));
        anti_dict.insert(ins_code, ins_word);
    }
    //Read the size of the BitArray of the zipped file.
    unsigned size;
    input.read((char*)(&size), sizeof(size));
    //Read bit_left of the BitArray.
    int zipped_file_bit_left;
    input.read((char*)(&zipped_file_bit_left), sizeof(zipped_file_bit_left));
    //Read the zipped file.
    auto* zipped_file = new unsigned char[size];
    input.read((char*)zipped_file, size * sizeof(unsigned char));
    input.close();
    BitArray zipped_file_array(code_bit_width, zipped_file, size, zipped_file_bit_left);
    BitArray output_file(4);
    string probe;
    for (unsigned i = 0; i < size; ++i) {
        probe.push_back(zipped_file_array[i]);
        auto word = anti_dict.find(probe);
        if (!word) continue;
        for (int j = basic_unit_size / 4 - 1; j >= 0; --j) {
            unsigned tmp = (*word >> (j * 4)) - ((*word >> ((j + 1) * 4)) << ((j + 1) * 4));
            output_file.push_back(tmp);
        }
        probe = "";
    }
    //Write output temporary file.
    ofstream output("zip_temp.tmp", ios::out | ios::binary);
    const auto& data = output_file.get_data();
    output.write((char*)data.c_array(), data.size() * sizeof(char));
    expand_files("zip_temp.zip", output_path);
    return true;
}
