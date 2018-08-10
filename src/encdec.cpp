#include "encdec.hpp"

void hash_to_point(std::string m, twistpoint_fp2_t& pt){
	hash_to_twist_subgroup(m, pt);
}

void hash_to_twist_subgroup(std::string m, twistpoint_fp2_t& pt){
	using boost::numeric_cast;
    using namespace utils;

	hashtotwistpoint(m, pt);
	auto x = cpp_int_to_scalar(twist_cofactor, twist_order);
	twistpoint_fp2_scalarmult_vartime(pt,pt, &x);
}

void hashtotwistpoint(std::string m, twistpoint_fp2_t& pt){
	using namespace boost::multiprecision;
    using namespace utils;

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
		int y = utils::fp2e_sqrt(sqr, t);
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
	using namespace std;
	using boost::numeric_cast;
	convert_context c;

	//if (!std::is_same<Integer, Int>::value){
	Int xx = numeric_cast<Int>(x);
	Int xy = numeric_cast<Int>(y);
	//}

	Int xxB = x % p;
	Int xyB = y % p;
	const auto x_bytes = to_bytes(numeric_cast<double>(xxB));
	const auto y_bytes = to_bytes(numeric_cast<double>(xyB));

	unsigned char* all_bytes = (unsigned char*) malloc(NUM_BYTES*2);

	std::copy(std::begin(x_bytes),std::end(x_bytes), all_bytes + (1*NUM_BYTES-x_bytes.size()));
	std::copy(std::begin(y_bytes), std::end(y_bytes), all_bytes  + (2*NUM_BYTES - y_bytes.size()));

	int i = 0;
	char* c_str = (char*) malloc(sizeof(all_bytes));
	for(auto i = 0; i < (sizeof(all_bytes)/sizeof(all_bytes[0])); ++i){
		c_str[i] = all_bytes[i];
	}

	std::string tmp(c_str);
	c.binary_number_to_doubles_fp2(tmp.c_str());

	memcpy(b->v, c.doublesFP2, sizeof(double) * 24);
}

void Set_xy_twistpoint(twistpoint_fp2_t & rop, fp2e_t x, fp2e_t y){
	twistpoint_fp2_affineset_fp2e(rop, x, y);
}

void pair(fp12e_t& result,curvepoint_fp_t c, twistpoint_fp2_t t){
    optate(result,t,c);
}