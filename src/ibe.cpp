// Copyright 2018 Doku Enterprise. All rights reserved.

#define MESSAGE ((const unsigned char *) "friedrich")
#define MESSAGE_LEN 9
#define CIPHERTEXT_LEND (crypto_secretbox_MACBYTES + MESSAGE_LEN)

#include <stdio.h>
#include <fstream>
#include "vars.hpp"
#include "ibe.hpp"
#include "json.hpp"

using namespace utils;

curvepoint_fp_t bn_curvegen = {{{{{1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}},
                                             {{{-2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}}},
                                             {{{1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}},
{{{0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}}}};

curvepoint_fp_t curvegen = {
	{{{{1., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.}}}}};

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
	curvepoint_fp_scalarmult_vartime(p, curvegen, &x);

    this->private_key.s = secret;
	memcpy(this->public_key.g1, p, sizeof(p));
}


void Ibe::extract(std::string id){
	twistpoint_fp2_t d, q;
	
	assert(id != "");
	std::cout << "EXTRACT: HASH TO POINT MODE" << std::endl;
	hash_to_point(id, q);

	cpp_int tmp = this->private_key.s;
	std::cout << "EXTRACT CPP_INT_TO" << std::endl;

	auto x = cpp_int_to_scalar(tmp, order);
	twistpoint_fp2_scalarmult_vartime(d, q, &x);

	memcpy(this->id_private_key.d, d, sizeof(d));
	memcpy(this->id_private_key.q, q, sizeof(q));
}

bool Ibe::test(){
	std::string msg = "powercat";
	unsigned char key[crypto_secretbox_KEYBYTES];
	unsigned char nonce[crypto_secretbox_NONCEBYTES];
	unsigned char ciphertext[CIPHERTEXT_LEND];

	crypto_secretbox_keygen(key);
	randombytes_buf(nonce, sizeof nonce);
	crypto_secretbox_easy(ciphertext, reinterpret_cast<const unsigned char*>(msg.c_str()), msg.length(), NULL, key);

	unsigned char decrypted[MESSAGE_LEN];
	if (crypto_secretbox_open_easy(decrypted, ciphertext, CIPHERTEXT_LEND, NULL, key) == 0) {
		return true;
	}
}

cipherdata Ibe::encrypt(std::string id, std::string msg){
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
	auto secr = cpp_int_to_scalar(secret, order);

	curvepoint_fp_t rp;
	curvepoint_fp_scalarmult_vartime(rp, curvegen, &secr);

	cpp_int p12 = p ^ p12;
	auto scalar = cpp_int_to_scalar(secret, p12); 
	fp12e_t er;
	fp12e_pow_vartime(er,g, &scalar);

    //TODO: Create a data stream that holds hashed values of
    // the points (q,rp, er) and stuff. Then read them into an array and 
	// seal them using sodium secret box.
	
	const auto q_bytes = to_bytes(q);
	const auto rp_bytes = to_bytes(rp);
	const auto er_bytes = to_bytes(er);

	/*FILE *fptr;
	fptr = fopen("/home/professor/Documents/Projects/crypto/pointENCDEC.txt","a+");
	twistpoint_fp2_print(fptr,q);
	curvepoint_fp_print(fptr,rp);
	fp12e_print(fptr,er);*/

	// Depends you may need to write a special function to marshall each point
	// this is a cheap version.
	json q_array(q_bytes);
	json rp_array(rp_bytes);
	json er_array(er_bytes);

	std::string xtt = q_array.dump() + rp_array.dump() + er_array.dump();
	std::string sk;
	sk = sha256(xtt);
	std::cout << "THIS IS THE ENC SECRET " << sk << std::endl;

	/*for(int i = 0; i < 32; ++i){
		sk[i] = xtt[i];
	}*/
	// All thats left is the sk
	// TODO: This may be very cheap and unsafe 
	

	//std::cout << "THIS IS A SECRET " << xtt.c_str() << std::endl;
	struct cipherdata d;
	d.ciphertext = (unsigned char*) calloc(CIPHERTEXT_LEN + msg.length() +1, sizeof(unsigned char));
	d.nonce = (unsigned char*) calloc(crypto_secretbox_NONCEBYTES, sizeof(unsigned char));
	d.cyrptolen = CIPHERTEXT_LEN + msg.length()+1;
	d.messagelen = msg.length()+1;

	randombytes_buf(d.nonce, sizeof(d.nonce));

	crypto_secretbox_easy(d.ciphertext, reinterpret_cast<const unsigned char*>(msg.c_str()),
		 d.messagelen,d.nonce, reinterpret_cast<const unsigned char*>(sk.c_str()));

	//std::cout << "THIS IS cipher " << cipher << std::endl;

	std::copy(std::begin(rp), std::end(rp), std::begin(d.rp));
	memcpy(d.rp, rp, sizeof(rp));

	//std::cout << "NONCE E: " << d.nonce << std::endl;
	//std::cout << "CIPHERTEXT E:" << d.ciphertext << std::endl;
	
	return d;
}

void Ibe::decrypt(idpk p, cipherdata data){
	using namespace nlohmann;
	
	std::cout << "DECRYPTING" << std::endl;
	fp12e_t point;
	// rp should be equal to curvepoint
	pair(point, data.rp, p.d);


	const auto b1 = to_bytes(p.q);
	const auto b2 = to_bytes(data.rp);
	const auto b3 = to_bytes(point);

	/*FILE *fptr;
	fptr = fopen("/home/professor/Documents/Projects/crypto/pointENCDEC.txt","a+");
	twistpoint_fp2_print(fptr,p.q);
	curvepoint_fp_print(fptr,data.rp);
	fp12e_print(fptr,point);*/

	// Depends you may need to write a special function to marshall each point
	// this is a cheap version.
	json a1(b1);
	json a2(b2);
	json a3(b3);

	std::string xtt = Marshal(p.q) + a2.dump() + a3.dump();
	std::string sk = sha256(xtt);
	std::cout << "THIS IS THE DEC SECRET " << sk << std::endl;

	unsigned char de[data.messagelen];
	if(crypto_secretbox_open_easy(de, data.ciphertext, data.cyrptolen, data.nonce, reinterpret_cast<const unsigned char*>(sk.c_str())) != 0){
		std::cout << "MESSAGE FORGERD" << std::endl;

	}else{
		std::cout << "DECRYPTED: " << de << std::endl;
	}

	//std::cout << "NONCE D: " << data.nonce << std::endl;
	//std::cout << "CIPHERTEXT D:" << data.ciphertext << std::endl;
}


std::array<fp6e_t, 6> GetFp2e(fp12e_t point){
	fp6e_t a = (fp6e_t)(point->m_a);
	fp6e_t b = (fp6e_t)(point->m_b);

	std::array<fp6e_t, 6> tmp = {a->m_a, a->m_b, a->m_c, 
					b->m_a, b->m_b, b->m_c};
	return tmp;
}

unsigned char* Marshal(fp12e_t point){
	auto out = (unsigned char*) malloc(NUM_BYTES*12);
	std::array<fp2e_t, 6> xs = GetFp2e(point);

	int i = 0;
	for(fp2e_t& x : xs){
		auto b = Marshal(x);
		memcpy(out + (i*NUM_BYTES*2), b, sizeof(b));
		i++;
	}
	return out;
}

unsigned char* Marshal(fp2e_t point){
	auto w = GetXY(point);

	const auto x_bytes = to_bytes(numeric_cast<double>(w.x));
	const auto y_bytes = to_bytes(numeric_cast<double>(w.y));

	unsigned char* all_bytes = (unsigned char*) malloc(NUM_BYTES*2);

	std::copy(std::begin(x_bytes),std::end(x_bytes), all_bytes + (1*NUM_BYTES-x_bytes.size()));
	std::copy(std::begin(y_bytes), std::end(y_bytes), all_bytes  + (2*NUM_BYTES - y_bytes.size()));
	return all_bytes;
}

unsigned char* Marshal(curvepoint_t point){
	auto out = (unsigned char*) malloc(NUM_BYTES * 2);
	fpe_t x, y;
	GetXY(x, y, point);
	auto tmp = Marshal(x);
	auto tmp2 = Marshal(y);

	memcpy(out, tmp, sizeof(tmp));
	memcpy(out + NUM_BYTES, y, sizeof(y));
}

unsigned char* Marshal(fpe_t point){
	auto out = (unsigned char*) malloc(NUM_BYTES);
	cpp_int tmp;

	convert_context c;
	tmp = c.doubles_to_int(tmp, point->v[0]);
	const auto bytes = to_bytes(numeric_cast<double>(tmp));
	memcpy(out + (NUM_BYTES - bytes.size()), bytes, sizeof(bytes));
	return out;
}

unsigned char* Marshal(twistpoint_fp2_t point){
	auto out = (unsigned char*) malloc((NUM_BYTES * 4) * sizeof(unsigned char));
	fp2e_t x, y;
	GetXY(x,y,point);

	auto serix = Marshal(x);
	auto seriy = Marshal(y);

	memcpy(out, serix, sizeof(serix));
	memcpy((out + (NUM_BYTES*2)),seriy, sizeof(seriy));
	return out;
}

void GetXY(fp2e_t r1, fp2e_t r2, twistpoint_fp2_t point){
	twistpoint_fp2_makeaffine(point);
	fp2e_set(r1, point->m_x[0]);
	fp2e_set(rt,point->m_y[0]);
}

struct intpair GetXY(fp2e_t point){
	using namespace boost::multiprecision;

	convert_context c;
	cpp_int x,y;
	auto doubles = point->v;
	x = c.doubles_fp2_to_int(x, doubles, 1);
	y = c.doubles_fp2_to_int(y, doubles, 0);
	struct intpair d;
	d.x = x;
	d.y = y;
	return d;
}

void GetXY(fpe_t r1, fpe_t r2, curvepoint_fp_t point){
	curvepoint_fp_makeaffine(point);
	fpe_set(r1, point->m_x[0]);
	fpe_set(r1, point->m_y[0]);
}