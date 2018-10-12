#include "conv.hpp"

using namespace utils;


Int p("65000549695646603732796438742359905742825358107623003571877145026864184071783");
Int SET1("6500054969564660373279643874235990574282535810762300357187714502686418407178");
Int SET2("45500384786952622612957507119651934019977750675336102500314001518804928850249");

const int v = 1868033; 
const int big_six = 6;
const int NUM_BYTES = 32;

// bytesToDoubles converts a binary, 
// big-endian number into 12 doubles that are
// in dclxvi's scalar format.
// TODO: check if my convesion works on line 21
void convert_context::binary_number_to_doubles(const char * m){
    using boost::numeric_cast;

    unsigned long long tmp = buff_to_integer(m);
    big_int x = numeric_cast<big_int>(tmp);
    
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
    std::string p1, p2;
  
    try{
        p1 = fs.substr(0,NUM_BYTES);
        p2 = fs.substr(fs.size() - NUM_BYTES);
    }catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
    }

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

unsigned long long buff_to_integer(const char * buffer)
{
    unsigned long long a;
    int byte = (strlen(buffer) - 1) * 8;
    for(int i = 0; i < strlen(buffer); ++i){
        a |= static_cast<unsigned long long>(buffer[i]) << byte;
        byte -= 8;
    }
    return a;
}
