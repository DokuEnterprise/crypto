#include "conv.hpp"
using namespace utils;
// bytesToDoubles converts a binary, 
// big-endian number into 12 doubles that are
// in dclxvi's scalar format.
void convert_context::binary_number_to_doubles(const char * m){
    using boost::numeric_cast;

    big_int x = bit_cast<big_int>(m);

    this->acc = x;

    this->v_power = big_six * v;
    this->acc = this->acc / this->v_power;
    this->tmp = this->acc % this->v_power;

    for(auto i = 1; i < 6; ++i){
        this->acc = this->acc / v;
        this->tmp = this->acc % v;
        this->doubles[i] = numeric_cast<double>(this->tmp);
    }

    this->acc = this->acc / this->v_power;
    this->tmp = this->acc % this->v_power;

    this->doubles[6] = numeric_cast<double>(this->tmp);
    for(auto i = 7; i < 11; ++i){
        this->acc = this->acc / v;
        this->tmp = this->acc % v;
        this->doubles[i] = numeric_cast<double>(this->tmp);
    }
    this->doubles[11] = numeric_cast<double>(this->acc);
}


// bytesToDoublesFP2 converts a pair of binary, big-endian values into 24
// doubles that are in dclxvi's F(p²) format.
void convert_context::binary_number_to_doubles_fp2(const char* x){
    std::string fs(x);

    //[:NUM_BYTES] and [NUM_BYTES:] in python
    std::string p1 = fs.substr(0,NUM_BYTES);
    std::string p2 = fs.substr(fs.size() - NUM_BYTES);

    const char* p1C = p1.c_str();
    const char* p2C = p2.c_str();

    this->binary_number_to_doubles(p1C);
    for(auto i = 0; i < 12; ++i){
        this->doublesFP2[2*i+1] = this->doubles[i];
    }
    this->binary_number_to_doubles(p2C);
    for(auto i = 0; i < 12; ++i){
        this->doublesFP2[2*i] = this->doubles[i];
    }
}
