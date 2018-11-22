#ifndef UTILS_H
#define UTILS_H

#include <assert.h>
#include <math.h>
#include <openssl/sha.h>
#include <stdio.h>
#include <asm/errno.h>
#include <type_traits>
#include <string.h>
#include <climits>

#include <stdint.h>
#include <stdlib.h>

extern "C"{
    #include <dclxvi/curvepoint_fp.h>
    #include <dclxvi/twistpoint_fp2.h>
    #include <dclxvi/fp2e.h>
    #include <dclxvi/fp12e.h>
    #include <dclxvi/scalar.h>
}


#include <boost/multiprecision/cpp_int.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multiprecision/integer.hpp>
#include <boost/cerrno.hpp>
#include <boost/numeric/conversion/cast.hpp> 

namespace utils{

    using byte = unsigned char;
    using cpp_int = boost::multiprecision::cpp_int;

    extern const fp2e_t fp2e_one;
    extern const fp2e_t fp2e_negOne;
    extern const fp2e_t fp2e_i;

    extern const scalar_t pMinus3Div4;
    extern const scalar_t pMinus1Div2;

    std::string sha256(std::string string);

    void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65]);

    int sha256_file(char * path, char outputBuffer[65]);

    // Set X and Y fp2e

    void fp2e_set_x(fp2e_t * a,
        const double * words);

    void fp2e_set_y(fp2e_t * a,
        const double * words);

    /*void fp2e_exp(fp2e_t rop,
        const fp2e_t op,
            const scalar_t exp);*/

    int fp2e_sqrt(fp2e_t rop,
        const fp2e_t a);


    int cpp_int_compare(cpp_int op, cpp_int val);
    std::array < uint64_t, 4 > cppint_to_words(cpp_int k_in, cpp_int bound);

    unsigned long long* cpp_int_to_scalar(cpp_int k_in, cpp_int bound);
    // Extra functions needed for crypto

}

#endif