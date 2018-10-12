#include <stdio.h>
#include <sodium/crypto_box.h>
#include <fstream>
#include "vars.hpp"
#include "ibe.hpp"
#include "json.hpp"

using namespace utils;

curvepoint_fp_t bn_curvegen = {{{{{1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}},
                                             {{{-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                                             {{{1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}},
{{{0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}}}};

Int order("65000549695646603732796438742359905742570406053903786389881062969044166799969");

// This needs to be cleaned up later
Int compute_vars(bool t_order){
	if(t_order){
		Int temp_o;
		temp_o = p * 2;
		temp_o = temp_o - order;
		temp_o = temp_o * order;
		return temp_o;

	}else{
		Int temp_co;
		temp_co = 2 * p;
		temp_co = temp_co - order;
		return temp_co;
	}
}

Int twist_order = compute_vars(1), twist_cofactor = compute_vars(0);


fp2e_t twistB;


Ibe::Ibe(){
	// twist_order = n(np-n)
	//this->twist_order = p * 2;
	//this->twist_order = twist_order - order;
	//this->twist_order = twist_order * order;

	Set_xy_fp2e(SET1, SET2,this->twistB);

	//this->twist_cofactor = 2 * p;
	//this->twist_cofactor = this->twist_cofactor - order;

	this->twist_order = twist_order;
	this->twist_cofactor = twist_cofactor;
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
	memcpy(this->public_key.g1, p, sizeof(p));
}


void Ibe::extract(std::string id){
	twistpoint_fp2_t d, q;
	// The hash to point function is getting the boost divide by zero error
	assert(id != "");
	std::cout << "EXTRACT: HASH TO POINT MODE" << std::endl;
	hash_to_point(id, q);

	cpp_int tmp = this->private_key.s;
	std::cout << "EXTRACT CPP_INT_TO" << std::endl;
	auto x = cpp_int_to_scalar(tmp, compute_vars(1));
	twistpoint_fp2_scalarmult_vartime(d, q, &x);

	memcpy(this->id_private_key.d, d, sizeof(d));
	memcpy(this->id_private_key.q, q, sizeof(q));
}

void Ibe::encrypt(std::string id, std::string msg){
	using namespace boost::multiprecision;
	using namespace boost::random;
	using namespace nlohmann;

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
	fp12e_pow_vartime(er,g,&x);

    //TODO: Create a data stream that holds hashed values of
    // the points (q,rp, er) and stuff. Then read them into an array and 
	// seal them using sodium secret box.
	
	const auto q_bytes = to_bytes(q);
	const auto rp_bytes = to_bytes(rp);
	const auto er_bytes = to_bytes(er);

	// Depends you may need to write a special function to marshall each point
	// this is a cheap version.
	json q_array(q_bytes);
	json rp_array(rp_bytes);
	json er_array(er_bytes);

	std::string xtt = q_array.dump() + rp_array.dump() + er_array.dump();
	std::string sk;
	xtt = sha256(xtt);

	for(int i = 0; i < 32; ++i){
		sk[i] = xtt[i];
	}
	// All thats left is the sk
	// TODO: This may be very cheap and unsafe 
	crypto_box_seal(nullptr, reinterpret_cast<const unsigned char*>(msg.c_str()),
		 24, reinterpret_cast<const unsigned char*>(sk.c_str()));
}