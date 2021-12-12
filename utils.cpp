#include "utils.hpp"
#include "HashMap.hpp"
#include <filesystem>
#include <fstream>
#include <cstring>
#include <iostream>
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
    -o Specify the name of output file.
settings:
    --size Specify basic unit size used in zip progress. This option will be effective only if "-c" is specified.
        The number following "--size" should be an positive integer, count by bit. It should be multiple of 4.
    --branch Specify the number of branches of Huffman tree used in zip progress. This option will be effective only if "-c" is specified.
        The number following "--branch" should be an positive integer.
    --display-tree Display the Huffman tree used to zip files. This option will be effective only if "-c" is specified.
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
        tmp.push_back(buf[i] >> 4);
        tmp.push_back(buf[i] & 0xf);
    }
    if (!(ans.size() % (basic_unit_size / 4))) {
        // Count by the number of 4-bit segments.
        int append_size = (2 * f_size / (basic_unit_size / 4) + 1) - 2 * f_size;
        for (int i = 0; i < append_size; ++i)
            tmp.push_back(0);
    }
    for (size_t i = 0; i < tmp.size(); i += basic_unit_size / 4) {
        int t_tmp = 0;
        for (int j = basic_unit_size / 4 - 1; j >= 0; --j) {
            t_tmp += tmp[i] << j;
        }
        ans.push_back(t_tmp);
    }
    delete[] buf;
    return ans;
}

PriorityQueue<TreeNode::NodeData> get_freq(const Vector<int> &data) {
    HashMap map;
    PriorityQueue<TreeNode::NodeData> ans;
    for (const auto &item: data) ++map[item];
    for (const auto &item: map)
        ans.push(TreeNode::NodeData(item.key, -item.value));
    return ans;
}

bool pack_up_files(const Vector<std::string>& input_files) {
    for (const auto& item : input_files) {
        try {
            filesystem::path file_path(item);
        }
        catch (const filesystem::filesystem_error& e) {
            auto err = string(e.what());
            cerr << "Huffman Zip: " << err.substr(err.find("No")) << endl;
            return false;
        }
    }
    ofstream output("zip_temp.tmp", ios::out | ios::binary);
    short num_of_input = input_files.size();
    //Write num of files.
    output.write((char*)(&num_of_input), sizeof(num_of_input));
    for (const auto& item : input_files) {
        filesystem::path file_path(item);
        auto file_name = filesystem::relative(file_path).c_str();
        short length = strlen(file_name);
        ifstream fin(item, ios::in | ios::binary);
        short file_size = filesystem::file_size(file_path);
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
    }
    return true;
}
