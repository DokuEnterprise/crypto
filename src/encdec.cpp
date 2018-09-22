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

void Set_xy_twistpoint(twistpoint_fp2_t & rop, fp2e_t x, fp2e_t y){
	twistpoint_fp2_affineset_fp2e(rop, x, y);
}

void pair(fp12e_t& result,curvepoint_fp_t c, twistpoint_fp2_t t){
    optate(result,t,c);
}