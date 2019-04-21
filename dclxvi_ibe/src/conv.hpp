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


#ifndef CONV_H
#define CONV_H

#include <array>
#include <memory>
#include <type_traits>
#include <iostream>
#include <iomanip>


#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/integer.hpp>
#include <boost/cerrno.hpp> 
#include <boost/numeric/conversion/cast.hpp>
#include "utils.hpp"

#include <algorithm>

// Conversion tools for IBE.

using byte = unsigned char;
using Int = boost::multiprecision::cpp_int;
using big_int = boost::multiprecision::int512_t;

extern Int p;
extern Int SET1;
extern Int SET2;

extern const int v; 
extern const int big_six;
extern const int NUM_BYTES;

class convert_context
{
public:
    big_int v_power;
    big_int tmp;
    big_int acc;
    double doubles[12];
    double doublesFP2[24];
    void binary_number_to_doubles(const char * m);
    void binary_number_to_doubles_fp2(const char* x);
    Int doubles_to_int(Int out, double *limbsIn);
    Int doubles_fp2_to_int(Int out, double *limbsIn, int phase);
private:
};

unsigned long long buff_to_integer(const char * buffer);

template<class T> 
std::array<byte, sizeof(T)>  to_bytes(const T& object)
{
    std::array<byte, sizeof(T)> bytes;

    auto begin = reinterpret_cast<const byte*>(std::addressof(object));
    auto end = begin + sizeof(T);
    std::copy(begin, end, std::begin(bytes));

    return bytes;
}

template<class T>
T& from_bytes(const std::array<byte,sizeof(T)>& bytes, T& object)
{
    // For reference http://en.cppreference.com/w/cpp/types/is_trivially_copyable
    static_assert(std::is_trivially_copyable<T>::value, "not a TriviallyCopyable type");

    byte* begin_object = reinterpret_cast<byte*>(std::addressof(object));
    std::copy(std::begin(bytes), std::end(bytes), begin_object);

    return object;
}

#endif