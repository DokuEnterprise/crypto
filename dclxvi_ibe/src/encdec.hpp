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

#include <boost/numeric/conversion/cast.hpp>

#include <boost/generator_iterator.hpp>

#include <boost/cerrno.hpp> 
#include <boost/random.hpp>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/integer.hpp>

#include "conv.hpp"
#include "utils.hpp"

using boost::numeric_cast;

using Int = boost::multiprecision::cpp_int;


extern curvepoint_fp_t bn_curvegen;
extern Int order;
extern fp2e_t twistB;

void hash_to_point(std::string m, twistpoint_fp2_t rop);
void hash_to_twist_subgroup(std::string m, twistpoint_fp2_t rop);
void hashtotwistpoint(std::string m, twistpoint_fp2_t rop);
void Set_xy_fp2e(Int x, Int y, fp2e_t& b);


void Set_xy_twistpoint(twistpoint_fp2_t rop, fp2e_t x, fp2e_t y);
void pair(fp12e_t result,curvepoint_fp_t c, twistpoint_fp2_t t);

#endif