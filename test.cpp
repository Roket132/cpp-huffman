//
// Created by Dima on 16.06.2018.
//

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <vector>
#include <utility>
#include <string>

#include "libhuffman/library.h"
#include "gtest/gtest.h"

std::string encode(std::string str, huffman &e) {
    e.add_freq(str);
    std::string encode;
    e.encode(str, encode);
    long long freq[256];
    e.get_freq(freq);
    return encode;
}

std::string decode(std::string encode, huffman &e) {
    huffman d;
    long long freq[256];
    e.get_freq(freq);
    d.load_freq(freq);
    std::string decode;
    d.decode(encode, decode, e.get_last());
    return decode;
}


TEST(correctness, easy) {
    std::string str = "00000000";
    huffman e;
    std::string enc = encode(str, e);
    std::string dec = decode(enc, e);
    EXPECT_EQ(str, dec);
}

TEST(correctness, null) {
    std::string str = "";
    huffman e;
    std::string enc = encode(str, e);
    std::string dec = decode(enc, e);
    EXPECT_EQ(str, dec);
}

TEST(correctness, one_letter) {
    std::string str = "a";
    huffman e;
    std::string enc = encode(str, e);
    std::string dec = decode(enc, e);
    EXPECT_EQ(str, dec);
}

TEST(correctness, diff_freq) {
    std::string str = "122333444455555666666";
    huffman e;
    std::string enc = encode(str, e);
    std::string dec = decode(enc, e);
    EXPECT_EQ(str, dec);
}

TEST(correctness, kycha_buqv) {
    std::string str = "fljwehgpfuh43qgpvhwohfp9843hfoi  2hfp9h2 0ruv2y489yb5vr8p92  y3rcpy2bp9ryp29 8yrp98  y3ruywepryp823r8293hyrf9q23yr9pc3y4t89vtyvwpoertyuiwqeyrviobwqr7tq43";
    huffman e;
    std::string enc = encode(str, e);
    std::string dec = decode(enc, e);
    EXPECT_EQ(str, dec);
}

TEST(correctness, broke_file) {
    std::string str = "if2j3403434jg30q0q3g34gp3gh38qgh39hv93nc94f-934q9gh3q98gh394fhwfuh4398fv983qjv89hq8dh923p8hfrhwegaser98w3rpaht8haw4p89t";
    huffman e;
    std::string enc = encode(str, e);
    enc += "000000000000000000000000000000000000";
    EXPECT_NO_THROW(decode(enc, e););
}

TEST(correctness, random_test) {
    std::string str;
    for (int i = 0; i < 10000000; i++) {
        int a = rand() % 256;
        str += char(a);
    }
    //std::cout << "str = " << str << std::endl;
    huffman e;
    std::string enc = encode(str, e);
    std::string dec = decode(enc, e);
    EXPECT_EQ(str, dec);
}


