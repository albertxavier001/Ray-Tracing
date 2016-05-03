#include "Ray.h"


Ray::Ray()
{
}



Ray::~Ray()
{
}

Eigen::Vector3f Ray::getEndPt(float t) const
{
	return ori + t * dir;
}
