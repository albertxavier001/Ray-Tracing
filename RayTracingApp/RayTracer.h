#pragma once
#include "Helper.h"
#include "Camera.h"
#include "Ray.h"
#include "Intersector.h"
#include "Scene.h"

#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/opencv.hpp"

class RayTracer
{
public:
	Helper hp;
	Camera &cam;
	Scene &scene;
	Intersector gIntersctor;
	//int nCausticPhotonsWanted, nIndirectPhotonsWanted, nLookup;
	//float maxDistSquared;
	int maxSpecularDepth;
public:
	RayTracer();
	RayTracer(Camera &cam, Scene &scene);
	~RayTracer();
	Ray genRay(Eigen::Vector2f p, Eigen::Vector2f offset);
	Eigen::Vector3f traceRay(Ray &ray, Scene &scene);
	Eigen::Vector3f getSample(int x, int y);
	void render(cv::Mat &dst);
};

