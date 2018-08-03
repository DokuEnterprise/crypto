#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/integer.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include "utils.hpp"

#include <algorithm>

// Conversion tools for IBE.

using Int = boost::multiprecision::cpp_int;
using big_int = boost::multiprecision::int512_t;

Int p("65000549695646603732796438742359905742825358107623003571877145026864184071783");
Int SET1("6500054969564660373279643874235990574282535810762300357187714502686418407178");
Int SET2("45500384786952622612957507119651934019977750675336102500314001518804928850249");

const int v = 1868033; 
const int big_six = 6;
const int NUM_BYTES = 32;

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
