#pragma once
#include "Helper.h"
class Ray
{
public:
	Helper hp;
	Eigen::Vector3f ori;
	Eigen::Vector3f dir;
	mutable float mint, maxt;
	int depth;

	Eigen::Vector3f invdir;
	Eigen::Vector3i sign;

public:
	Ray();
	Ray(const Eigen::Vector3f &p, const Eigen::Vector3f &v, float start = 0.f, float end = INFINITY, int d = 0)
		: ori(p), dir(/*hp.normalise4v*/(v).normalized()), mint(start), maxt(end), depth(d) 
	{ 
		invdir = (1 / dir.array()).matrix();
		sign[0] = (invdir.x() < 0);
		sign[1] = (invdir.y() < 0);
		sign[2] = (invdir.z() < 0);
	};
	Ray(const Eigen::Vector3f &origin, const Eigen::Vector3f &direction, const Ray &parent,
		float start, float end = INFINITY)
		: ori(origin), dir(direction), mint(start), maxt(end),
		depth(parent.depth + 1) { }
	~Ray();
	Eigen::Vector3f getEndPt(float t) const;
};

