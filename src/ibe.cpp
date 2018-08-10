#include <stdio.h>
#include "ibe.hpp"

using namespace utils;

curvepoint_fp_t bn_curvegen = {{{{{1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}},
                                             {{{-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                                             {{{1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}},
{{{0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}}}};

Int order("65000549695646603732796438742359905742570406053903786389881062969044166799969");

Int twist_order, twist_cofactor;
fp2e_t twistB;


Ibe::Ibe(){
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

void Ibe::setup(){
	using namespace boost::multiprecision;
	using namespace boost::random;

	curvepoint_fp_t p;

	mt19937 mt;
    uniform_int_distribution<cpp_int> ui(0, order);
	
	auto secret = ui(mt);
	auto x = cpp_int_to_scalar(secret, order);
	curvepoint_fp_scalarmult_vartime(p, bn_curvegen, &x);

    this->private_key.s = secret;
	this->public_key.g1 = p;
}

void Ibe::extract(std::string id){
	twistpoint_fp2_t d, q;
	hash_to_point(id, q);
	cpp_int tmp = this->private_key.s;
	auto x = cpp_int_to_scalar(tmp, twist_order);
	twistpoint_fp2_scalarmult_vartime(d, q, &x);

	this->id_private_key.d = &d;
	this->id_private_key.q = &q;
}

void Ibe::encrypt(std::string id, std::string msg){
	using namespace boost::multiprecision;
	using namespace boost::random;

	twistpoint_fp2_t q;
	hash_to_point(id, q);

	fp12e_t g;
	pair(g, this->public_key.g1, q);

	mt19937 mt;
    uniform_int_distribution<cpp_int> ui(0, order);
	
	auto secret = ui(mt);
	auto x = cpp_int_to_scalar(secret, order);

	curvepoint_fp_t rp;
	curvepoint_fp_scalarmult_vartime(rp, bn_curvegen, &x);

	fp12e_t er;

}