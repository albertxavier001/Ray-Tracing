#include "BSDF.h"
#include "material.h"
#include <algorithm>
#include <cstdlib>
#include "Scene.h"
BSDF::BSDF()
{
}

BSDF::BSDF(
	Eigen::Vector3f				_n,
	Eigen::Vector3f				_spc,
	Eigen::Vector3f				_dfs,
	Eigen::Vector3f				_rfr,
	int				_shiness,
	float			_eta,
	int				_modeltype
	)
	:
	nn(_n.normalized()),
	specular(_spc),
	diffuse(_dfs),
	refract(_rfr),
	shiness(_shiness),
	etat(_eta),
	modelType(_modeltype)
{

}

BSDF::~BSDF()
{
}

Eigen::Vector3f BSDF::f(const Eigen::Vector3f &wiW, const Eigen::Vector3f &woW) const {
	Helper hp;
	Eigen::Vector3f c(0,0,0);

	/// lambertion
	if (modelType == LAMBERTIAN) // lambert
	{
		return diffuse;
	}
	else if (modelType == PHONG) // blinn phone
	{
		if (/*SameHemisphere(wiW, woW, nn)*/1) {	//ignore btdf
			Eigen::Vector3f n = nn.normalized();
			if (/*arma::dot(n, woW)*/n.dot(woW) < 0) {
				n *= -1.f;
			}
			if (specular[0] != 0 ||
				specular[1] != 0 ||
				specular[2] != 0) {
				Eigen::Vector3f H = (wiW.normalized() + woW.normalized()).normalized();
				float s = pow(fabs(H.dot(nn)), shiness);
				c += specular * s;
			}

			if (
				diffuse[0] != 0 ||
				diffuse[1] != 0 ||
				diffuse[2] != 0
				) {
				c += diffuse * fabs(/*arma::dot(wiW, nn)*/wiW.dot(nn));
			}
		}
		else {
		}

		return c;
	}
	
	return c;
}

float randomf2(float LO, float HI)
{
	float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return r3;
}

Eigen::Vector3f  CosWeightedRandomHemisphereDirection2(const Eigen::Vector3f &_n)
{
	
	
	float Xi1 = (float)rand() / (float)RAND_MAX;
	float Xi2 = (float)rand() / (float)RAND_MAX;

	float  theta = acos(sqrt(1.0 - Xi1));
	float  phi = 2.0 * M_PI * Xi2;

	float xs = sinf(theta) * cosf(phi);
	float ys = cosf(theta);
	float zs = sinf(theta) * sinf(phi);

	Eigen::Vector3f  y = _n;
	Eigen::Vector3f  h = y;
	if (fabs(h[0]) <= fabs(h[1]) && fabs(h[0]) <= fabs(h[2]))
		h[0] = 1.0;
	else if (fabs(h[1]) <= fabs(h[0]) && fabs(h[1]) <= fabs(h[2]))
		h[1] = 1.0;
	else
		h[2] = 1.0;


	Eigen::Vector3f  x = /*arma::normalise(arma::cross(h, y))*/(h.cross(y)).normalized();
	Eigen::Vector3f  z = /*arma::normalise(arma::cross(x, y))*/x.cross(y).normalized();

	Eigen::Vector3f  direction = xs * x + ys * y + zs * z;
	direction = /*arma::normalise(direction)*/direction.normalized();
	return direction;
}


Eigen::Vector3f BSDF::sample(const Eigen::Vector3f &woW, Eigen::Vector3f &wiW, int type) {
	//printf("brdf sample\n");

	if (type == SPECULAR) {
		//if (modeltype == 0)//lambert
		//{
		//	wiW[0] = randomf2(-1,1);
		//	wiW[1] = randomf2(-1, 1);
		//	wiW[2] = randomf2(-1, 1);
		//	wiW = hp.normalise4v(wiW);
		//	return diffuse;
		//}
		Eigen::Vector3f  n = (nn.normalized());
		if (n.dot(woW) < 0) {
			n *= -1.f;
		}
		float proj_len = n.dot(woW);
		wiW = n * 2 * proj_len - (woW.normalized());
		//wiW = CosWeightedRandomHemisphereDirection2(n);
		//Eigen::Vector3f ret = f(wiW, woW);
		//return ret;
		return specular;
	}
	else if (type == REFRACT) {
#ifdef _DEBUG
		if (etat > 1.f)
		{
			std::cout << "asdasd" << std::endl;
		}
#endif // _DEBUG
		
		Eigen::Vector3f wo = woW.normalized();
		Eigen::Vector3f n = nn.normalized();
		float cos = wo.dot(n);
		float sin = sqrtf(1.f - cos * cos);
		Eigen::Vector3f x, y, z;
		float sint, cost;
		if (cos > 0.f) { // refract from outside
			// System.out.println("bigger than zero");
			sint = std::min(1.f, sin / etat);
			cost = sqrtf(1.f - sint * sint);
			//z = hp.normalise4v(hp.cross4v(wo, n));
			z = wo.cross(n).normalized();
			y = n*(-1.f);
			//x = hp.normalise4v(hp.cross4v(y, z));
			x = y.cross(z).normalized();
			//wiW = hp.normalise4v(-x*sint + y*cost);
			wiW = (-x*sint + y*cost).normalized();
			//wiW = CosWeightedRandomHemisphereDirection2(n);
			// debug
			//wiW = -woW;
		}
		else { // refract from inside
			if (sin * etat > 1)
			{
				sint = (sin);
				cost = sqrtf(1. - sin * sin);
				z = wo.cross(-n);
				y = -n;
				x = y.cross(z);
				//wiW = hp.normalise4v(x*sint + y*cost);
				// debug
				float proj_len = -n.dot(woW);
				wiW = (-n * 2 * proj_len - woW.normalized()).normalized();
				//wiW = CosWeightedRandomHemisphereDirection2(n);
			}
			else
			{
				//sint = std::min(1.f, sin * etat);
				sint = sin * etat;
				cost = sqrtf(1. - sint * sint);
				z = wo.cross(-n);
				y = n;
				x = y.cross(z);
				wiW = (x*sint + y*cost).normalized();
				//wiW = CosWeightedRandomHemisphereDirection2(n);
				// debug
				//wiW = -woW;
				
			}
			
		}
		return refract;
	}
}

