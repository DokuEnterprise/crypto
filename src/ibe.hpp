#pragma once

#include <bn256/init.h>
#include <bn256/curvepoint_fp.h>
#include <bn256/twistpoint_fp2.h>
#include <bn256/points.h>
#include <bn256/fp12e.h>
#include <bn256/comfp12e.h>
#include <bn256/randompoints.h>

#include <bn256/ate_optate.h>
#include <bn256/eta_tate.h>
#include <bn256/cometa_comtate.h>
#include <bn256/cpucycles.h>

/*#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/multiprecision/cpp_int.hpp>*/

#include <gmp.h>
#include <time.h>

//using Int = boost::multiprecision::cpp_int;
//namespace mp = boost::multiprecision; 
//namespace rd = boost::random;

// /Int order("65000549695646603732796438742359905742570406053903786389881062969044166799969");
using byte = unsigned char;

typedef curvepoint_fp_t bn256_cp;
typedef twistpoint_fp2_t twis;

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
    void extract(byte id[]);
    mpriv private_key;
    mpublic public_key; 
private:
    
};


