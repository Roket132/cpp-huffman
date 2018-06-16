#include "library.h"

#include <string>
#include <cmath>
#include <set>
#include <iostream>
#include <unordered_map>
#include <algorithm>

huffman::huffman() {
    clear();
}

int huffman::encode(std::string &str, std::string &str_out_buf, bool last) {
    if (!tree_b) {
        bit = 0;
        n = 0;
        tree_b = true;
        tree = build_tree(freq, 256);
    }

    for (char i : str) {
        std::string code = tree[i];
        for (char j : code) {
            if (j == '1') {
                bit |= (1 << n);
            }
            n++;
            if (n == 8) {
                str_out_buf += bit;
                n = 0;
                bit = 0;
            }
        }
    }

    if (last) {
        if (n) {
            str_out_buf += bit;
        }
    }
    last_ind = n;
    return n;
}


void huffman::decode(std::string &str, std::string &str_out_buf, int last) {
    if (!back_tree_b) {
        bit_h = 0;
        pow = 0;
        pw[0] = 1;
        for (int i = 1; i < 32; i++)
            pw[i] = pw[i - 1] * 31;
        tree = build_tree(freq, 256);
        back_tree_b = true;
        build_back_tree();
    }

    for (char ch : str) {
        for (int it = 0; it < 8; it++) {
            int b = ch & (1 << it);
            if (b) {
                bit_h += 2 * pw[pow++];
            } else {
                bit_h += pw[pow++];
            }
            if (pow >= (size_t)mn) {
                if (back_tree.count(bit_h)) {
                    str_out_buf.push_back(back_tree[bit_h]);
                    bit_h = 0;
                    pow = 0;
                }
            }
        }
    }

    if (last) {
        bit_h = 0;
        pow = 0;
        for (int i = last; i < 8; i++) {
            bit_h += pw[pow++];
            if (back_tree.count(bit_h)) {
                bit_h = 0;
                pow = 0;
                str_out_buf.pop_back();
            }
        }
    }
}

void huffman::add_freq(std::string &str) {
    frequency(str, freq);
}

void huffman::frequency(std::string &str, long long *a) {
    for (char i : str) {
        a[(unsigned char) i]++;
    }
}

void huffman::clear() {
    tree_b = false;
    back_tree_b = false;

    for (long long &i : freq) {
        i = 0;
    }
    tree.clear();
    back_tree.clear();
}

std::unordered_map<char, std::string> huffman::build_tree(long long *a, int n) {
    a['a']++, a['b']++;
    std::multiset<std::pair<long long, std::string>> node;
    for (int i = 0; i < n; i++) {
        if (a[i]) {
            auto ch = static_cast<unsigned char>(i);
            std::string res;
            res += ch;
            node.insert({a[i], res});
        }
    }

    std::unordered_map<char, std::string> res_tree;

    while (node.size() > 1) {
        auto first = node.begin();
        auto second = node.begin();
        second++;
        std::pair<long long, std::string> fp = *first;
        std::pair<long long, std::string> sp = *second;
        node.erase(fp);
        node.erase(sp);

        for (auto it : fp.second) {
            res_tree[it] += "0";
        }
        for (auto it : sp.second) {
            res_tree[it] += "1";
        }
        std::pair<int, std::string> new_pair({static_cast<const int &>(fp.first + sp.first), fp.second + sp.second});
        node.insert(new_pair);
    }
    for (auto &it : res_tree) {
        std::reverse(it.second.begin(), it.second.end());
    }

    a['a']--, a['b']--;
    return res_tree;
}

void huffman::get_freq(long long *get_freq) {
    for (int i = 0; i < 256; i++) {
        get_freq[i] = freq[i];
    }
}

void huffman::load_freq(long long *load) {
    for (int i = 0; i < 256; i++) {
        freq[i] = load[i];
    }
}

void huffman::build_back_tree() {
    mn = 100;
    back_tree.clear();
    back_tree.reserve(2024 * 2024 * 10);
    for (auto it : tree) {
        unsigned long long bit = 0;
        unsigned long long pow = 1;
        mn = ((size_t) mn > it.second.size() ? it.second.size() : mn);
        for (char i : it.second) {
            bit += (i - '0' + 1) * pow;
            pow *= 31;
        }
        back_tree[bit] = it.first;
    }
}

int huffman::get_last() {
    return last_ind;
}
