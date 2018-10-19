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

<<<<<<< HEAD
=======

struct g2{
	twistpoint_fp2_t p;
};

struct g1{
	curvepoint_fp_t *p;
};

>>>>>>> 57ab4a55645fd2c6db2ad5f3d70b2b3253dc6024
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
 
struct cipherdata{
    curvepoint_fp_t rp;
    size_t messagelen;
    size_t cyrptolen;
    unsigned char ciphertext[];
    unsigned char nonce[];
};
typedef struct MasterPrivateKey mpriv;
typedef struct MasterPublicKey mpublic;
typedef struct IdentityPrivateKey idpk;

class Ibe
{
public:
    Ibe();
    void setup();
    void extract(std::string id);
    cipherdata encrypt(std::string id, std::string msg);
    void decrypt(idpk *p, cipherdata data);
    mpriv private_key;
    mpublic public_key; 
    idpk id_private_key;
    fp2e_t twistB;
    Int twist_cofactor;
private:
    Int twist_order;
};

void Set_xy_twistpoint(twistpoint_fp2_t & rop, fp2e_t x, fp2e_t y);

void hash_to_point(std::string m, twistpoint_fp2_t& pt);
void hash_to_twist_subgroup(std::string m, twistpoint_fp2_t& pt);
void hashtotwistpoint(std::string m, twistpoint_fp2_t& pt);

#endif