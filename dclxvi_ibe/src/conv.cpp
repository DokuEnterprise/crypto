/*
 * Copyright (c) 2019 Doku Enterprise
 * Author: Friedrich Doku
 * -----
 * Last Modified: Wednesday November 28th 2018 5:04:24 am
 * -----
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 3 of the License, or
 *   (at your option) any later version.
 *   This program is distributed in the hope that it will be useful
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

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

// doublesToInt sets out to a value decoded from 12 doubles in dclxvi's format.
// dclxvi stores values as described in [1], section 4.1.
Int convert_context::doubles_to_int(Int out, double *limbsIn){
    using boost::numeric_cast;

    auto limbs = limbsIn;
    out = numeric_cast<int64_t>(limbs[0]);

    this->v_power = 1868033;
    this->tmp = numeric_cast<int64_t>(limbs[1] * 6);
    this->tmp = this->tmp * this->v_power;
    out = out + this->tmp;

    int i = 2;
    for(int64_t factor = 6; factor <= 36; factor *=6){
        for(int j = 0; i < 5; ++j){
            this->v_power = this->v_power * 1868033;
            this->tmp = numeric_cast<int64_t>(limbs[i]) * factor;
            this->tmp = this->tmp * this->v_power;
            out = out + this->tmp;
            i++;
        }
    }
    out = out % p;
    return out;
}

// doublesFP2ToInt set out to a value decoded from 24 doubles in dclxvi's F(p²)
// format. dclxvi stores these values as pairs of the scalars where those
// scalars are in the form described in [1], section 4.1. The words of the two
// values are interleaved and phase (which must be either 0 or 1) determines
// which of the two values is decoded.
Int convert_context::doubles_fp2_to_int(Int out, double *limbsIn, int phase){
    auto limb2 = limbsIn;
    double limbs[12];

    for(int i = 0; i < 12; ++i){
        limbs[i] = limb2[2*i+phase];
    }
    return this->doubles_to_int(out, limbs);
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
