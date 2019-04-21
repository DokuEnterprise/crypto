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


#include "encdec.hpp"
#include "vars.hpp"

void hash_to_point(std::string m, twistpoint_fp2_t rop){
	hash_to_twist_subgroup(m, rop);
}

// Twist cofactor and twist order are undefined this needs to be
// fixed.
void hash_to_twist_subgroup(std::string m, twistpoint_fp2_t rop){
	using boost::numeric_cast;
    using namespace utils;

	hashtotwistpoint(m,rop);
	Int t_co = p * 2;
	//std::cout << "hash_to_twist_subgroup " << twist_cofactor << std::endl;
	//std::cout << "hash_to_twist_subgroup " << twist_order << std::endl; 
	auto x = cpp_int_to_scalar(twist_cofactor, twist_order);
	twistpoint_fp2_scalarmult_vartime(rop,rop, x);
	twistpoint_fp2_makeaffine(rop);
}

void hashtotwistpoint(std::string m, twistpoint_fp2_t rop){
	using namespace boost::multiprecision;
    using namespace utils;

    fp2e_t one;
    fp2e_setone(one);

	std::string hxx = sha256(m.append("0"));
	std::string hxy = sha256(m.append("1"));

	size_t xx = std::hash<std::string>{}(hxx);
	size_t xy = std::hash<std::string>{}(hxy);
	
	Int boostx = numeric_cast<Int>(xx);
	Int boosty = numeric_cast<Int>(xy);

	fp2e_t x;
	
	Set_xy_fp2e(boostx, boosty, x);

	for(;;){
		fp2e_t xxx, t, sqr;
		fp2e_square(xxx,x);
		fp2e_mul(xxx, xxx, x);

		fp2e_add(t,xxx, twistB);
		int y = utils::fp2e_sqrt(sqr, t);
		if(y != 0){
			twistpoint_fp2_t pt;
			Set_xy_twistpoint(pt,x,sqr);
			twistpoint_fp2_makeaffine(pt);
			twistpoint_fp2_set(rop, pt);
			break;
		}
		fp2e_add(x,x, one);
	}
}


void Set_xy_fp2e(Int x, Int y, fp2e_t& b){
	using namespace std;
	using boost::numeric_cast;
	convert_context c;

	Int xxB = x % p;
	Int xyB = y % p;

	// May need to be changed
	// https://stackoverflow.com/questions/50465041/how-to-convert-boostmultiprecisioncpp-int-tofrom-an-array-of-byte
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

void Set_xy_twistpoint(twistpoint_fp2_t rop, fp2e_t x, fp2e_t y){
	twistpoint_fp2_affineset_fp2e(rop, x, y);
}

void pair(fp12e_t result,curvepoint_fp_t c, twistpoint_fp2_t t){
	curvepoint_fp_t tmp1;
	twistpoint_fp2_t tmp2;

	curvepoint_fp_set(tmp1, c);
	twistpoint_fp2_set(tmp2, t);

	curvepoint_fp_makeaffine(tmp1);
	twistpoint_fp2_makeaffine(tmp2);
    optate(result,tmp2,tmp1);
}