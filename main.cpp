#include "libhuffman/library.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <unordered_map>
#include <ctime>

void encode(char *file_in, char *file_out) {
    unsigned int encode_t = clock();
    std::ifstream in(file_in, std::ios::in | std::ios::binary);
    if (!in) {
        std::cerr << "File not found\n";
        return;
    }

    char buf[2048576];
    huffman e;
    while (in.read(buf, sizeof(buf)).gcount() > 0) {
        std::string res = "";
        res.append(buf, (size_t) in.gcount());
        e.add_freq(res);
    }

    std::ofstream out(file_out, std::ofstream::binary);

    int sz = 256;
    out.write((char *) &sz, sizeof(sz));

    long long freq[256];
    e.get_freq(freq);

    for (int i = 0; i < 256; i++) {
        out.write((char *) &i, sizeof(i));
        out.write((char *) &freq[i], sizeof(freq[i]));
    }
    in.close();

    std::ifstream in2(file_in, std::ios::in | std::ios::binary);
    std::string str_out_buf = "";

    int last;
    while (in2.read(buf, sizeof(buf)).gcount() > 0) {
        std::string str = "";
        str.append(buf, (size_t) in2.gcount());

        e.encode(str, str_out_buf, false);

        if (str_out_buf.size() > 1024 * 1024 * 10) {
            out << str_out_buf;
            str_out_buf = "";
        }
    }
    std::string str = "";
    last = e.encode(str, str_out_buf, true);
    if (str_out_buf.size()) {
        out << str_out_buf;
    }
    out.seekp(0, out.beg);
    out.write((char *) &last, sizeof(last));
    in2.close();
    out.close();
    unsigned int encode_te = clock();
    std::cout << "encode: " << (encode_te - encode_t) / CLOCKS_PER_SEC << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

void decode(char *file_in, char *file_out) {
    unsigned int decode_t = clock();
    std::ifstream in(file_in, std::ios::in | std::ios::binary);
    if (!in) {
        std::cerr << "File not found\n";
        return;
    }
    std::ofstream out(file_out, std::ofstream::binary);
    huffman d;

    int last;
    in.read((char *) &last, sizeof(last));
    long long freq[256];
    for (int i = 0; i < 256; i++) {
        int ind;
        long long fr;
        in.read((char *) &ind, sizeof(ind));
        in.read((char *) &fr, sizeof(fr));
        freq[ind] = fr;
    }
    d.load_freq(freq);


    char buf[2048576];
    std::string res_out_buf;
    while (in.read(buf, sizeof(buf)).gcount() > 0) {
        std::string str;
        str.append(buf, (size_t) in.gcount());
        d.decode(str, res_out_buf, 0);
        if (res_out_buf.size() > 2048576) {
            out << res_out_buf;
            res_out_buf = "";
        }
    }
    std::string str = "";
    d.decode(str, res_out_buf, last);

    if (res_out_buf.size() > 0) {
        out << res_out_buf;
    }
    in.close();
    out.close();
    unsigned int decode_te = clock();
    std::cout << "decode: " << (decode_te - decode_t) / CLOCKS_PER_SEC << std::endl;
}

int main(int argc, char *argv[]) {

    setlocale(LC_ALL, "Russian");

    if (argc != 4) {
        std::cerr << "use e/d in out\n";
        return 0;
    } else {
        std::cerr << argv[1] << " " << argv[2] << " " << argv[3] << std::endl;
    }

    std::string mode = argv[1];

    if (mode == "e") {
        encode(argv[2], argv[3]);
    } else if (mode == "d") {
        decode(argv[2], argv[3]);
    } else {
        std::cerr << "use e/d in out\n";
    }
    return 0;
}