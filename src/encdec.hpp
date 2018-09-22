#ifndef ENCDEC_H
#define ENCDEC_H

extern "C"{
    #include <dclxvi/curvepoint_fp.h>
    #include <dclxvi/twistpoint_fp2.h>
    #include <dclxvi/fp2e.h>
    #include <dclxvi/optate.h>
    #include <dclxvi/fp12e.h>
}

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>

#include <boost/random.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/integer.hpp>

#include "conv.hpp"
#include "utils.hpp"



extern curvepoint_fp_t bn_curvegen;
extern Int order;
extern Int twist_order;
extern Int twist_cofactor;
extern fp2e_t twistB;

void hash_to_point(std::string m, twistpoint_fp2_t& pt);
void hash_to_twist_subgroup(std::string m, twistpoint_fp2_t& pt);
void hashtotwistpoint(std::string m, twistpoint_fp2_t& pt);

template<class Integer>
void Set_xy_fp2e(Integer x, Integer y, fp2e_t& b){
	using namespace std;
	using boost::numeric_cast;
	convert_context c;

	//if (!std::is_same<Integer, Int>::value){
	Int xx = numeric_cast<Int>(x);
	Int xy = numeric_cast<Int>(y);
	//}

	Int xxB = x % p;
	Int xyB = y % p;
	const auto x_bytes = to_bytes(numeric_cast<double>(xxB));
	const auto y_bytes = to_bytes(numeric_cast<double>(xyB));

	unsigned char* all_bytes = (unsigned char*) malloc(NUM_BYTES*2);

	std::copy(std::begin(x_bytes),std::end(x_bytes), all_bytes + (1*NUM_BYTES-x_bytes.size()));
	std::copy(std::begin(y_bytes), std::end(y_bytes), all_bytes  + (2*NUM_BYTES - y_bytes.size()));

	int i = 0;
	char* c_str = (char*) malloc(sizeof(all_bytes));
	for(auto i = 0; i < (sizeof(all_bytes)/sizeof(all_bytes[0])); ++i){
		c_str[i] = all_bytes[i];
	}

	std::string tmp(c_str);
	c.binary_number_to_doubles_fp2(tmp.c_str());

	memcpy(b->v, c.doublesFP2, sizeof(double) * 24);
}
void Set_xy_twistpoint(twistpoint_fp2_t& rop, fp2e_t x, fp2e_t y);
void pair(fp12e_t& result,curvepoint_fp_t c, twistpoint_fp2_t t);

#endif