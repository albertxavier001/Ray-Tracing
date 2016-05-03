#include "Helper.h"


Helper::Helper()
{
}


Helper::~Helper()
{
}



inline int Helper::clamp(int value, int low, int up) {
	if (value > up)	return up;
	if (value < low) return low;
	return value;
}



//
//float Helper::length(arma::fvec &v)
//{
//	return sqrt(arma::accu(v%v));
//}