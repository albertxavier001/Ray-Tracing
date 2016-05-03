#pragma once
#define _USE_MATH_DEFINES
#define DEBUG
//#define  _SMALL_SCENE
#include "armadillo"
#include <vector>
#include <ctime>
#include <string>
#include <sstream>
#include <Eigen/Core>
#include <Eigen/Dense>

//typedef arma::fvec4 Eigen::Vector3f;
//typedef arma::fvec3 Vc3;
//typedef arma::fvec4 Eigen::Vector3f;
//typedef arma::fvec3 Pt3;
//typedef arma::fvec2 Pt2;
//typedef arma::fmat::fixed<4, 4> Mx4;
//typedef arma::fvec3 Eigen::Vector3f;
//using namespace Eigen;

class Helper
{
public:
	Helper();
	~Helper();

	inline int clamp(int value, int low, int up);
	//inline arma::fvec4 cross4v(const arma::fvec4 &_v1, const arma::fvec4 &_v2)
	//{
	//	arma::fvec3 v1, v2;
	//	v1.at(0) = _v1.at(0);
	//	v1.at(1) = _v1.at(1);
	//	v1.at(2) = _v1.at(2);
	//	v2.at(0) = _v2.at(0);
	//	v2.at(1) = _v2.at(1);
	//	v2.at(2) = _v2.at(2);

	//	arma::fvec3 v = arma::cross(v1, v2);
	//	arma::fvec4 vout;
	//	vout.at(0) = v.at(0);
	//	vout.at(1) = v.at(1);
	//	vout.at(2) = v.at(2);
	//	vout.at(3) = 0.f;
	//	//vout.at(3) = 1.f;
	//	return vout;
	//}
	
	inline Eigen::Vector3f cross4v(const Eigen::Vector3f &_v1, const Eigen::Vector3f & _v2, Eigen::Vector3f &v3)
	{
		Eigen::Vector3f v = _v1.cross(_v2).normalized();
		return v;
	}

	/*inline arma::fvec4 normalise4(const arma::fvec4 &_v)
	{
		arma::fvec4 v0 = _v;
		arma::fvec3 v;
		v[0] = _v[0];
		v[1] = _v[1];
		v[2] = _v[2];
		float norm = arma::norm(v);
		if (fabs(norm) < 1e-6) return _v;
		else
		{
			v0 /= norm;
			v0[3] = 1.f;
		}
		return v0;
	}*/

	//inline arma::fvec4 normalise4v(const arma::fvec4 &_v)
	//{
	//	arma::fvec4 v0 = _v;
	//	arma::fvec3 v;
	//	v[0] = _v[0];
	//	v[1] = _v[1];
	//	v[2] = _v[2];
	//	float norm = arma::norm(v);
	//	if (fabs(norm) < 1e-6) return _v;
	//	else
	//	{
	//		v0 /= norm;
	//		//v0[3] = 1.f;
	//	}
	//	return v0;
	//}

	//inline arma::fvec4 normalise4v(const arma::fvec4 &_v)
	//{
	//	arma::fvec4 v0 = _v;
	//	arma::fvec3 v;
	//	v[0] = _v[0];
	//	v[1] = _v[1];
	//	v[2] = _v[2];
	//	float norm = arma::norm(v);
	//	if (fabs(norm) < 1e-6) return _v;
	//	else
	//	{
	//		v0 /= norm;
	//		//v0[3] = 1.f;
	//	}
	//	return v0;
	//}


	//float length(arma::fvec &v);
};

