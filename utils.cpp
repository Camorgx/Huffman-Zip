//
// Created by 曹高翔 on 2021/11/22.
//

#include "utils.hpp"
using namespace std;

void string_split(vector<string>& ans, const string& source, const string& split) {
    auto pos2 = source.find(split);
    int pos1 = 0;
    while(pos2 != string::npos) {
        ans.push_back(source.substr(pos1, pos2 - pos1));
        pos1 = pos2 + split.size();
        pos2 = source.find(split, pos1);
    }
    if(pos1 != source.length())
        ans.push_back(source.substr(pos1));
}

string get_help(const string& program_name) {
    string ans = "(Huffman_zip 1.0, using Huffman algorithm to zip files. Usage:\n";
    string tmp = program_name, t_tmp;
    for (auto i = tmp.rbegin(); i != tmp.rend(); ++i)
        if (*i != '\\' && *i != '/') t_tmp.push_back(*i);
        else break;
    tmp.clear();
    for (auto i = t_tmp.rbegin(); i != t_tmp.rend(); ++i)
        tmp.push_back(*i);
    ans += tmp + R"( [options] [settings]
options:
    -f Specify a file.
    -c Zip given file.
    -x Unzip given file.
    -h Show this help.
settings:
    --size Specify basic unit size used in zip progress. This option will be ignored if "-x" is specified.
    --branch Specify the number of branches of Huffman tree used in zip progress. This option will be ignored if "-x" is specified.
)";
    return move(ans);
}
