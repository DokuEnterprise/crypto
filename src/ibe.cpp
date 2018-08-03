#include <stdio.h>
#include "ibe.hpp"

Int twist_order, twist_cofactor;
fp2e_t twistB;

MasterKey::MasterKey(){
	// twist_order = n(np-n)
	this->twist_order = p * 2;
	this->twist_order = twist_order - order;
	this->twist_order = twist_order * order;

	Set_xy_fp2e(SET1, SET2,this->twistB);

	this->twist_cofactor = 2 * p;
	this->twist_cofactor = this->twist_cofactor - order;

	twist_order = this->twist_order;
	twist_cofactor = this->twist_cofactor;
	Set_xy_fp2e(SET1,SET2,twistB);
}

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

// Fix this: bit_cast may not be a correct conversion
void hash_to_twist_subgroup(std::string m, twistpoint_fp2_t& pt){
	using boost::numeric_cast;

	hashtotwistpoint(m, pt);
	scalar_t c {bit_cast<scalar_t>(twist_cofactor)};
	twistpoint_fp2_scalarmult_vartime(pt,pt, c);
}

void hashtotwistpoint(std::string m, twistpoint_fp2_t& pt){
	using namespace boost::multiprecision;
    fp2e_t one;
    fp2e_setone(one);

	std::string hxx = sha256(m.append("0"));
	std::string hxy = sha256(m.append("1"));

	size_t xx = std::hash<std::string>{}(hxx);
	size_t xy = std::hash<std::string>{}(hxy);

	fp2e_t x;
	Set_xy_fp2e(xx, xy, x);

	for(;;){
		fp2e_t xxx, t, sqr;
		fp2e_square(xxx,x);
		fp2e_mul(xxx, xxx, x);

		fp2e_add(t,xxx, twistB);
		int y = fp2e_sqrt(sqr, t);
		if(y != 0){
			twistpoint_fp2_t pt;
			Set_xy_twistpoint(pt,x,sqr);
			twistpoint_fp2_makeaffine(pt);
			break;
		}
		fp2e_add(x,x, one);
	}
}

template<class Integer>
void Set_xy_fp2e(Integer x, Integer y, fp2e_t& b){
	convert_context c;

	const char *xx = bit_cast<const char*>(x);
	const char *xy = bit_cast<const char*>(y);	
	std::string stpoint = std::string(xx) + std::string(xy);
	const char *point = stpoint.c_str();

	c.binary_number_to_doubles_fp2(point);

	memcpy(b->v, c.doublesFP2, sizeof(double) * 24);
}

void Set_xy_twistpoint(twistpoint_fp2_t & rop, fp2e_t x, fp2e_t y){
	twistpoint_fp2_affineset_fp2e(rop, x, y);
}
