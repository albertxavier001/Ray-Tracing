#include "Intersector.h"


Intersector::Intersector()
{
}

Intersector::Intersector(
	int				id,
	Eigen::Vector3f				_p,
	Eigen::Vector3f				_n,
	float			_re,
	Eigen::Vector3f				spc,
	Eigen::Vector3f				dfs,
	Eigen::Vector3f				rfr,
	int				shiness,
	float			eta,
	Eigen::Vector3f				_areaLight,/*=Eigen::Vector3f("0 0 0")*/
	int				modeltype
	):id(id),
	p(_p),
	n(_n),
	re(_re),
	areaLight(_areaLight),
	bsdf(BSDF(_n, spc, dfs, rfr, shiness, eta, modeltype))
{

}


Intersector::~Intersector()
{
}




bool Intersector::intersect(
	const Ray						&ray,
	float							&tHit,
	float							&rayEpsilon,
	Intersector						&inter,
	const std::vector<struct MESH> &shapes,
	int								sIdx,
	int								fIdx
	)
{


	

	

	/// intersect with a triangle mesh
	Eigen::Vector3f 
		p1 = shapes[sIdx].faces[fIdx].geo.v[0], 
		p2 = shapes[sIdx].faces[fIdx].geo.v[1],
		p3 = shapes[sIdx].faces[fIdx].geo.v[2];
	


	Eigen::Vector3f e1 = p2 - p1;
	Eigen::Vector3f e2 = p3 - p1;
	Eigen::Vector3f s1 =/* hp.cross4v*/(ray.dir.cross(e2));
	float divisor = /*arma::dot*/(s1.dot(e1));

	if (divisor == 0.) return false;
	float invDivisor = 1.f / divisor;

	Eigen::Vector3f d = ray.ori - p1;
	float b1 = /*arma::dot*/(d.dot(s1)) * invDivisor;
	if (b1 < 0. || b1 > 1.) return false;

	Eigen::Vector3f s2 = /*hp.cross4v*/(d.cross(e1));
	float b2 = /*arma::dot*/(ray.dir.dot(s2))* invDivisor;
	if (b2 < 0. || b1 + b2 > 1.) return false;

	float t = /*arma::dot*/(e2.dot(s2)) * invDivisor;
	if (t < ray.mint || t > ray.maxt) return false;

	/// xxx

	tHit = t;
	rayEpsilon = 1e-3f * tHit;

	Eigen::Vector3f dp1 = p1 - p3, dp2 = p2 - p3;
	Eigen::Vector3f n = /*hp.normalise4v*/(/*hp.cross4v*/(dp1.cross(dp2)).normalized());

	if (/*arma::dot*/(ray.dir.dot(n)) > 0)
	{
		n *= -1.f;
	}
	
	Eigen::Vector3f nn = 
		(shapes[sIdx].faces[fIdx].geo.n[0] +
		shapes[sIdx].faces[fIdx].geo.n[1] +
		shapes[sIdx].faces[fIdx].geo.n[2])/3;

	inter = Intersector(sIdx, ray.getEndPt(t), nn, rayEpsilon,
		shapes[sIdx].faces[fIdx].mtl.specular,//spc,
		shapes[sIdx].faces[fIdx].mtl.diffuse,//dfs,
		shapes[sIdx].faces[fIdx].mtl.refract,//refract,
		shapes[sIdx].faces[fIdx].mtl.shininess,//material.shininess,
		shapes[sIdx].faces[fIdx].mtl.Ni,//material.ior,//material./*eta*/dissolve,
		shapes[sIdx].faces[fIdx].mtl.ambient,//ambient,
		shapes[sIdx].faces[fIdx].mtl.type//modelType
		);
	return true;
}






/// ray from camera
bool Intersector::intersectScene(
		const Ray					&ray,
		Intersector					&intesector,
		const std::vector<struct MESH> &shapes
	)
{
	Intersector curIntersect;
	float t, re;
	float tHit = INFINITY;
	bool hitted = false;
	for (int i = 0; i < shapes.size(); ++i)
	{
		// intersect a shape
		
		/// intersect with a sphere
		if (shapes[i].type == SPHERE)
		{
			float t;
			if (!shapes[i].intersect(ray, t))
			{
				continue;
			}
			// is intersected
			Eigen::Vector3f pp = ray.ori + t*ray.dir;

			Eigen::Vector3f nn = (pp - shapes[i].center).normalized();
			curIntersect = Intersector(i, ray.getEndPt(t), nn, 1e-3f * t,
				shapes[i].faces[0].mtl.specular,//spc,
				shapes[i].faces[0].mtl.diffuse,//dfs,
				shapes[i].faces[0].mtl.refract,//refract,
				shapes[i].faces[0].mtl.shininess,//material.shininess,
				shapes[i].faces[0].mtl.Ni,//material.ior,//material./*eta*/dissolve,
				shapes[i].faces[0].mtl.ambient,//ambient,
				shapes[i].faces[0].mtl.type//modelType
				);

			hitted = true;
			if (t < tHit)
			{
				tHit = t;
				intesector = curIntersect;
			}
		}
		else
		{
			/// intersect a triangle mesh
			if (!shapes[i].bbox.isIntersect(ray)) continue;
			for (int j = 0; j < shapes[i].size(); ++j)
			{
				// intersect a triangle
				if (intersect(ray, t, re, curIntersect, shapes, i, j))
				{
					hitted = true;
					if (t < tHit)
					{
						tHit = t;
						intesector = curIntersect;
					}
				}
			}
		}
	}

	return hitted;
}

