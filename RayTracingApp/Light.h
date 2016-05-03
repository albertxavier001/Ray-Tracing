#pragma once
#include "tiny_obj_loader.h"
#include <string>
#include "Helper.h"
#include "Intersector.h"
#include "Scene.h"
class Intersector;
class Scene;
class Light
{
public:
	Helper hp;
	const Helper hpc;
	std::string name;
	int type;
	Eigen::Vector3f intensity;
	Eigen::Vector3f lightPos;
	tinyobj::shape_t *shape;
	int lid;

public:
	Light();
	~Light();
	
	Eigen::Vector3f sampleLight(Scene& scene, Intersector &gIntersctor, Eigen::Vector3f p, Eigen::Vector3f& wi, float rayEpsilon) const;


};

