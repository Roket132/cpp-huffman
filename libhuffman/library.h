#ifndef LIBHUFFMAN_LIBRARY_H
#define LIBHUFFMAN_LIBRARY_H

#include <string>
#include <unordered_map>
#include <map>


struct huffman{
    huffman();
    int encode(std::string &s, std::string &str_out_buf, bool last = true);
    void decode(std::string &s, std::string &str_out_buf, int last);
    void add_freq(std::string &str);
    void get_freq(long long *freq);
    void load_freq(long long *load);
    void clear();
    int get_last();
private:
    std::unordered_map<char, std::string> tree;
    std::unordered_map<unsigned long long, char> back_tree;
    char bit;
    int last_ind, n, mn;
    long long freq[256];
    bool tree_b, back_tree_b;
    unsigned long long bit_h, pow, pw[32];

    void frequency(std::string &str, long long *a);
    std::unordered_map<char,std::string> build_tree(long long *a, int n);
    void build_back_tree();

};



#endif