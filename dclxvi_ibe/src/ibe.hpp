/*
 * Copyright (c) 2019 Doku Enterprise
 * Author: Friedrich Doku
 * -----
 * Last Modified: Wednesday December 12th 2018 8:04:39 am
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


#ifndef IBE_H
#define IBE_H

extern "C"{
    #include <dclxvi/curvepoint_fp.h>
    #include <dclxvi/twistpoint_fp2.h>
    #include <dclxvi/fp2e.h>
    #include <dclxvi/fp12e.h>
}

#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>

#include <boost/random.hpp>
#include <boost/cerrno.hpp> 
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/integer.hpp>

#include <sodium/crypto_secretbox.h>
#include <sodium/randombytes.h>

#include <gmp.h>
#include <string>
#include <cstdio>
#include <stdlib.h>
#include <fstream>
#include <functional>
#include <array>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <time.h>
#include <assert.h>  
#include "conv.hpp"
#include "encdec.hpp"
#include "utils.hpp"

//namespace mp = boost::multiprecision; 
//namespace rd = boost::random;

// Needed for sodium
#define CIPHERTEXT_LEN (crypto_secretbox_MACBYTES)

extern curvepoint_fp_t bn_curvegen;

extern Int order;

using Int = boost::multiprecision::cpp_int;

struct MasterPublicKey{
    curvepoint_fp_t g1;
};

struct MasterPrivateKey{
    Int s;
};

struct IdentityPrivateKey{
    twistpoint_fp2_t d;
    twistpoint_fp2_t q;
};

struct intpair{
    Int x;
    Int y;
};

struct TestPoints{
    twistpoint_fp2_t p1;
    curvepoint_fp_t p2;
    fp12e_t p3;
};

struct cipherdata{
    curvepoint_fp_t rp;
    size_t messagelen;
    size_t cyrptolen;
    // May not work with all compilers: the size 
    // of the struct must be known at compile time for some compilers
    unsigned char* nonce;
    unsigned char* ciphertext;
};

typedef struct TestPoints testpoints;
typedef struct MasterPrivateKey mpriv;
typedef struct MasterPublicKey mpublic;
typedef struct IdentityPrivateKey idpk;
typedef struct intpair intpair;

class Ibe
{
public:
    Ibe();
    void setup();
    void extract(std::string id);
    bool test();
    cipherdata encrypt(std::string id, std::string msg);
    void decrypt(idpk p, cipherdata data);
    mpriv private_key;
    mpublic public_key; 
    idpk id_private_key;
    fp2e_t twistB;
    Int twist_cofactor;
private:
    Int twist_order;
    //unsigned char key[crypto_secretbox_KEYBYTES];
};


struct intpair GetXY(fp2e_t point);

std::vector<fp2e_t> GetFp2e(fp12e_t point);

// Marshall Functions
std::vector<unsigned char> Marshal(fp12e_t point);
std::vector<unsigned char> Marshal(fp2e_t point);
std::vector<unsigned char> Marshal(curvepoint_fp_t point);
std::vector<unsigned char> Marshal(fpe_t point);
std::vector<unsigned char> Marshal(twistpoint_fp2_t point);


void GetXY(fp2e_t r1, fp2e_t r2, twistpoint_fp2_t point);

void GetXY(fpe_t r1, fpe_t r2, curvepoint_fp_t point);

#endif