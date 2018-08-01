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

void MasterKey::extract(std::string id){

}

twistpoint_fp2_t hashtotwistpoint(std::string msg){
	//using namespace boost::multiprecision;

    fp2e_t one;
    fp2e_setone(one);

	std::string hxx = sha256(msg.append("0"));
	std::string hxy = sha256(msg.append("1"));

	//size_t xx = std::hash<std::string>{}(hxx);
	//size_t xy = std::hash<std::string>{}(hxy);
	
	unsigned char *data =  (unsigned char)malloc(NUM_BYTES*2);

	fp2e_t x;
	memcpy(x[0].v[0], dxx, sizeof(double) * 24);
	x := new(fp2e).SetXY(xx, xy)

	for {
		xxx := new(fp2e).Square(x)
		xxx.Mul(xxx, x)

		t := new(fp2e).Add(xxx, twistB)
		y := new(fp2e).Sqrt(t)
		if y != nil {
			pt := new(twistPoint).SetXY(x, y)
			pt.MakeAffine()
			return pt
		}

		x.Add(x, one)
}

