#pragma once
#include "Helper.h"
#include <vector>
#include "tiny_obj_loader.h"
#include "BSDF.h"
#include "armadillo"
#include "Ray.h"
#include "Scene.h"
class Intersector
{
public:
	int id;
	Eigen::Vector3f p;
	Eigen::Vector3f n;
	float re;
	BSDF bsdf;
	Eigen::Vector3f areaLight;
	Helper hp;
public:
	Intersector();
	//Intersector(int id, Eigen::Vector3f _p, Eigen::Vector3f _n, float _re, Eigen::Vector3f spc, Eigen::Vector3f dfs, Eigen::Vector3f rfr, int shiness, float eta, Eigen::Vector3f _areaLight = Eigen::Vector3f("0 0 0"));
	Intersector(int id, Eigen::Vector3f _p, Eigen::Vector3f _n, float _re, Eigen::Vector3f spc, Eigen::Vector3f dfs, Eigen::Vector3f rfr, int shiness, float eta, Eigen::Vector3f _areaLight,/*=Eigen::Vector3f("0 0 0")*/ int modeltype);
	~Intersector();
	//bool intersect(const Ray &ray, float &tHit, float &rayEpsilon, Intersector &inter, /*const tinyobj::shape_t &shape, */ /*const std::vector<tinyobj::material_t> &materials, */ const std::vector<struct TRIANGLE_MESH> &shapes, int id, int ind1, int ind2, int ind3, int fInd);
	bool intersect(const Ray &ray, float &tHit, float &rayEpsilon, Intersector &inter, /*const tinyobj::shape_t &shape, */ /*const std::vector<tinyobj::material_t> &materials, */ const std::vector<struct MESH> &shapes, int id, int fInd);
	//bool intersectScene(const Ray &ray, Intersector &intesector, const std::vector<tinyobj::shape_t> &shapes, const std::vector<tinyobj::material_t> &materials);
	bool intersectScene(const Ray &ray, Intersector &intesector, const std::vector<struct MESH> &shapes /*const std::vector<tinyobj::shape_t> &shapes, */ /*const std::vector<tinyobj::material_t> &materials */);
};

