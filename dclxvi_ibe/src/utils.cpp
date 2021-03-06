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


#include "utils.hpp"

namespace utils{

    using byte = unsigned char;
    using cpp_int = boost::multiprecision::cpp_int;

    const fp2e_t fp2e_one = {{{1., 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};
    const fp2e_t fp2e_negOne = {{{-1., 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};
    const fp2e_t fp2e_i = {{{0, 1., 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}};

    const scalar_t pMinus3Div4 = {0x86172b1b17822599, 0x7b96e234482d6d67, 0x6a9bfb2e18613708, 0x23ed4078d2a8e1fe};
    const scalar_t pMinus1Div2 = {0xc2e56362f044b33, 0xf72dc468905adacf, 0xd537f65c30c26e10, 0x47da80f1a551c3fc};

    std::string sha256(const std::string str)
    {
        using namespace std;

        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);
        stringstream ss;
        for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss << hex << setw(2) << setfill('0') << (int)hash[i];
        }
        return ss.str();
    }

    void sha256_hash_string(unsigned char hash[SHA256_DIGEST_LENGTH], char outputBuffer[65]){
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
        }

        outputBuffer[64] = 0;
    }

    int sha256_file(char * path, char outputBuffer[65]){
        FILE * file = fopen(path, "rb");
        if (!file) return -534;

        byte hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init( & sha256);
        const int bufSize = 32768;
        char * buffer = new char[bufSize];
        int bytesRead = 0;
        if (!buffer) return ENOMEM;
        while ((bytesRead = fread(buffer, 1, bufSize, file))){
            SHA256_Update( & sha256, buffer, bytesRead);
        }
        SHA256_Final(hash, & sha256);

        sha256_hash_string(hash, outputBuffer);
        fclose(file);
        free(buffer);
        return 0;
    }

    // Set X and Y fp2e

    void fp2e_set_x(fp2e_t * a,
        const double * words) {
        memcpy(&a[0]->v[0], words, sizeof(double) * 24);
    }
    void fp2e_set_y(fp2e_t * a,
        const double * words) {
        memcpy(&a[0]->v[0], words, sizeof(double) * 24);
    }

    /*void fp2e_exp(fp2e_t rop,
        const fp2e_t op,
            const scalar_t exp){
        fp2e_t dummy;
        unsigned int startbit;

        startbit = scalar_scanb(exp);
        fp2e_set(dummy, op);
        fp2e_set(rop, op);
        int i;
        for (i = startbit; i > 0; i--){
            fp2e_square(rop, rop);
            if (scalar_getbit(exp, i - 1))
                fp2e_mul(rop, rop, dummy);
        }
    }*/

    int fp2e_sqrt(fp2e_t rop,
        const fp2e_t a){
        fp2e_t a1, alpha, a0, x0, b;

        fp2e_exp(a1, a, pMinus3Div4);
        fp2e_mul(alpha, a1, a);
        fp2e_mul(alpha, a1, alpha);

        fp2e_conjugate(a0, alpha);
        fp2e_mul(a0, a0, alpha);

        if (fp2e_iseq(a0, fp2e_negOne)){
            return 0;
        }

        fp2e_mul(x0, a1, a);
        if (fp2e_iseq(alpha, fp2e_negOne)){
            fp2e_mul(rop, fp2e_i, x0);
            return 1;
        } else {
            fp2e_add(b, fp2e_one, alpha);
            fp2e_exp(b, b, pMinus1Div2);
            fp2e_mul(rop, b, x0);
            return 1;
        }
    }

    int cpp_int_compare(cpp_int op, cpp_int val){
        if(op < val){
            return -1;
        }
        if(op > val){
            return 1;
        }
        if(op == val){
            return 0;
        }
    }

    // Divide by zero boost problem.
    // This function needs to be fixed

    std::array < uint64_t, 4 > cppint_to_words(cpp_int k_in, cpp_int bound){
        cpp_int k;
        if ((k_in.sign() < 0) || (cpp_int_compare(k_in, bound) >= 0)) {
            // This is where the problem is k_in and bound are both 0 for some reason
            k = k_in % bound;
        } else {
            k = k_in;
        }
        if ((sizeof(abs(k)) * CHAR_BIT) > 256) {
            std::cerr << "Scalar value is too large" << std::endl;
        }

        std::array < uint64_t, 4 > words;
        for (uint64_t & x: words) {
            x = k.convert_to < uint64_t > ();
            k = k >> 64;
        }
        return words;
    }

    unsigned long long* cpp_int_to_scalar(cpp_int k_in, cpp_int bound){
        using boost::numeric_cast;
        //std::cout << "cpp_int_to_scalar" << k_in << std::endl;
        //std::cout << "cpp_int_to_scalar" << bound << std::endl;
        auto words = cppint_to_words(k_in, bound);
        unsigned long long* x = (unsigned long long*) malloc(4 * sizeof(unsigned long long));
        for(int i = 0; i < words.size(); ++i){
            x[i] = numeric_cast<unsigned long long>(words[i]);
        }
        return x;
    }
}

