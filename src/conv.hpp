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
    Int doubles_to_int(Int out, double limbsIn);
    Int doubles_fp2_to_int(Int out, double limbsIn, int phase);
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