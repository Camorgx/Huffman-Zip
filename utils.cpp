#include "utils.hpp"
#include <filesystem>
#include <fstream>
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
    for (int i = 0; i < f_size; ++i) {
        tmp.push_back(buf[i] >> 4);
        tmp.push_back(buf[i] & 0xf);
    }
    if (!(ans.size() % (basic_unit_size / 4))) {
        // Count by the number of 4-bit segments.
        int append_size = (2 * f_size / (basic_unit_size / 4) + 1) - 2 * f_size;
        for (int i = 0; i < append_size; ++i)
            tmp.push_back(0);
    }
    for (int i = 0; i < tmp.size(); i += basic_unit_size / 4) {
        int t_tmp = 0;
        for (int j = basic_unit_size / 4 - 1; j >= 0; --j) {
            t_tmp += tmp[i] << j;
        }
        ans.push_back(t_tmp);
    }
    delete[] buf;
    return ans;
}
