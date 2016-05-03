#pragma once
#include <string>
#include "tiny_obj_loader.h"
#include <vector>
#include <iostream>
#include "Light.h"
#include "Intersector.h"
#include "Ray.h"

class Light;
class Intersector;


enum SHAPE_TYPE{
	SPHERE,
	TRIANGLE
};

struct BBOX{
	float xMin;
	float xMax;
	float yMin;
	float yMax;
	float zMin;
	float zMax;


	inline bool isIntersect(const Ray &r) const
	{
		Eigen::Vector3f min(xMin, yMin, zMin);
		Eigen::Vector3f max(xMax, yMax, zMax);

		float tmin = (min.x() - r.ori.x()) / r.dir.x();
		float tmax = (max.x() - r.ori.x()) / r.dir.x();

		if (tmin > tmax) std::swap(tmin, tmax);

		float tymin = (min.y() - r.ori.y()) / r.dir.y();
		float tymax = (max.y() - r.ori.y()) / r.dir.y();

		if (tymin > tymax) std::swap(tymin, tymax);

		if ((tmin > tymax) || (tymin > tmax))
			return false;

		if (tymin > tmin)
			tmin = tymin;

		if (tymax < tmax)
			tmax = tymax;

		float tzmin = (min.z() - r.ori.z()) / r.dir.z();
		float tzmax = (max.z() - r.ori.z()) / r.dir.z();

		if (tzmin > tzmax) std::swap(tzmin, tzmax);

		if ((tmin > tzmax) || (tzmin > tmax))
			return false;

		if (tzmin > tmin)
			tmin = tzmin;

		if (tzmax < tmax)
			tmax = tzmax;

		return true;
	}
	
};

enum MODELTYPE{
	LAMBERTIAN,
	//SPECULAR,
	PHONG
};

struct MATERIAL {
	/// material parameterS
	Eigen::Vector3f ambient;
	Eigen::Vector3f diffuse;
	Eigen::Vector3f specular;
	Eigen::Vector3f refract;
	float shininess;
	float Ni;
	int type;
};

struct GEOMETRY{
	/// vertex and normal
	Eigen::Vector3f v[3];
	Eigen::Vector3f n[3];
};

struct TRIANGLE {
	GEOMETRY geo;
	MATERIAL mtl;

};

struct MESH{
	/// name
	std::string name;
	int type;
	int id;
	/// sphere
	Eigen::Vector3f center;
	float radius;
	/// triangles
	std::vector<struct TRIANGLE> faces;
	/// bounding box
	BBOX bbox;

	inline int size() const { return faces.size(); };

	inline Eigen::Vector3f getCenter(const std::vector<struct TRIANGLE> &faces)
	{
		Eigen::Vector3f center;
		center[0] = center[1] = center[2] = 0;
		for (int i = 0; i < faces.size() ; ++i) 
		{
			center += faces[i].geo.v[0];
			center += faces[i].geo.v[1];
			center += faces[i].geo.v[2];
		}
		center /= (faces.size() * 3);
		return center;
	};

	inline float getRadius(const std::vector<struct TRIANGLE> &faces, const Eigen::Vector3f &center)
	{
		float r = 0;
		for (int i = 0; i < faces.size(); ++i)
		{
			r += (faces[i].geo.v[0] - center).norm();
			r += (faces[i].geo.v[1] - center).norm();
			r += (faces[i].geo.v[2] - center).norm();
		}
		r /= (faces.size() * 3);
		return r;
	}

	inline bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) const
	{
		float discr = b * b - 4 * a * c;
		if (discr < 0) return false;
		else if (discr == 0) x0 = x1 = -0.5 * b / a;
		else {
			float q = (b > 0) ?
				-0.5 * (b + sqrt(discr)) :
				-0.5 * (b - sqrt(discr));
			x0 = q / a;
			x1 = c / q;
		}
		if (x0 > x1) std::swap(x0, x1);

		return true;
	}


	bool intersect(const Ray &ray, float &t) const
	{
		float t0, t1; // solutions for t if the ray intersects 

		// analytic solution
		Eigen::Vector3f L = ray.ori - center;
		float a = ray.dir.dot(ray.dir);
		float b = 2 * ray.dir.dot(L);
		float c = L.dot(L) - radius*radius;
		if (!solveQuadratic(a, b, c, t0, t1)) return false;
		if (t0 > t1) std::swap(t0, t1);

		if (t0 < 0) {
			t0 = t1; // if t0 is negative, let's use t1 instead 
			if (t0 < 0) return false; // both t0 and t1 are negative 
		}

		t = t0;

		return true;
	}

};

class Scene
{
public:
	Scene();
	~Scene();
	void loadObj(std::string objFileName, std::string mtlFileName);
	void genShapse(const std::vector<tinyobj::shape_t> &_shapes, std::vector<tinyobj::material_t> &_materials, std::vector<struct MESH> &shapes);
	std::vector<tinyobj::shape_t> _shapes;
	std::vector<struct MESH> shapes;
	std::vector<tinyobj::material_t> _materials;
	std::vector<Light> lights;

	Intersector *gIntersector;
};

