#pragma once
#include "Helper.h"
class BSDF
{
public:
	const static int DIFFUSE = 0;
	const static int SPECULAR = 1;
	const static int REFRACT = 2;

	Eigen::Vector3f nn, ng;
	Eigen::Vector3f sn, tn;
	int nBxDFs;
	float etat;
	Eigen::Vector3f specular;
	Eigen::Vector3f diffuse;
	Eigen::Vector3f refract;
	int shiness;
	int modelType;

	Helper hp;
public:
	BSDF();
	BSDF(Eigen::Vector3f _n, Eigen::Vector3f _spc, Eigen::Vector3f _dfs, Eigen::Vector3f _rfr, int _shiness, float _eta, int _modeltype);
	~BSDF();
	Eigen::Vector3f f(const Eigen::Vector3f &wiW, const Eigen::Vector3f &woW) const;
	Eigen::Vector3f sample(const Eigen::Vector3f &woW, Eigen::Vector3f &wiW, int type);

};

