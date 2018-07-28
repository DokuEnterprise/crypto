#include <stdio.h>
#include "ibe.hpp"

void MasterKey::setup(){
    curvepoint_fp_t d;

    gmp_randstate_t state;
	gmp_randinit_default(state);
	unsigned long int seed;
	seed = (unsigned long int)time(NULL);
	gmp_randseed_ui(state, seed);

	mpz_t scalar;
	mpz_init(scalar);
	
	do
		mpz_urandomm(scalar, state, p);
	while(!mpz_cmp_ui(scalar, 0));

	curvepoint_fp_mul(d, curve_gen, scalar);

	mpz_clear(scalar);
	gmp_randclear(state);

    // Got random point
    curvepoint_fp_print(stdout, d);
    this->private_key.s = &scalar;
    this->public_key.g1 = &d;
}

void MasterKey::extract(byte id[]){
    
}