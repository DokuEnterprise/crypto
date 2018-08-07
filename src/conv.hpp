#ifndef CONV_H
#define CONV_H

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/integer.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "utils.hpp"

#include <algorithm>

// Conversion tools for IBE.

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
private:
};

#endif