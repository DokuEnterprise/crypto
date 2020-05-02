#pragma once

#include <gmpxx.h>
#include <stdio.h>
#include <mcl/bls12_381.hpp>
#include <openssl/sha.h>


using namespace mcl::bn;


inline void gen(mpz_t rop, char* str)
{
    mpz_t n;
    gmp_randstate_t state;
    unsigned long int seed; 
    mpz_init(rop);
    mpz_init(n);
  
    // Base 10 string
  	mpz_set_str(n, str, 10 );

    seed = 1;

    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);
    mpz_urandomm(rop, state, n);

    //mpz_clear(rop);
    mpz_clear(n);
    gmp_randclear(state);
}

inline auto serialize(G2 q, G1 rp, GT er){
    char buf1[4096];
    char buf2[4096];
    char buf3[4096]; // I could be wrong about the sizes

    std::string s1 = rp.serializeToHexStr();
    std::string s2 = q.serializeToHexStr();
    std::string s3 = er.serializeToHexStr();

    auto rez = s1 + s2 + s3;
    return rez;
}

inline void sha256(char *string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}