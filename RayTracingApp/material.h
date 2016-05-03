#pragma once 
#include "armadillo"
#include "Helper.h"

const static int DIFFUSE = 0;
const static int SPECULAR = 1;
const static int REFRACT = 2;

inline bool SameHemisphere(const Eigen::Vector3f &w, const Eigen::Vector3f &wp, const Eigen::Vector3f &nn) {
	/*Helper hp;
	float s1 = arma::dot(hp.normalise4v(w), nn);
	float s2 = arma::dot(hp.normalise4v(wp), nn);
	return arma::dot(hp.normalise4v(w), nn) * arma::dot(hp.normalise4v(wp), nn) > 0.f;*/
	float s1 = w.normalized().dot(nn);
	float s2 = wp.normalized().dot(nn);
	return s1*s2>0.f;
}