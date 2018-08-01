#pragma once


#include <dclxvi/curvepoint_fp.h>
#include <dclxvi/twistpoint_fp2.h>
#include <dclxvi/fp12e.h>
#include <dclxvi/cpucycles.h>

/*#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>*/

#include <boost/multiprecision/cpp_int.hpp>

#include <gmp.h>
#include <string>
#include <functional>
#include <time.h>
#include "utils.hpp"

//using Int = boost::multiprecision::cpp_int;
//namespace mp = boost::multiprecision; 
//namespace rd = boost::random;

// /Int order("65000549695646603732796438742359905742570406053903786389881062969044166799969");

typedef curvepoint_fp_t bn256_cp;
typedef twistpoint_fp2_t twis;
const int NUM_BYTES = 32;


struct g2 {
	twistpoint_fp2_t p;
};

struct g1 {
	curvepoint_fp_t p;
};

struct MasterPublicKey{
    bn256_cp *g1;
};

struct MasterPrivateKey{
    mpz_t *s;
};

typedef struct MasterPrivateKey mpriv;
typedef struct MasterPublicKey mpublic;

class MasterKey
{
public:
    MasterKey();
    ~MasterKey();
    void setup();
    void extract(std::string id);
    mpriv private_key;
    mpublic public_key; 
private:
    
};


