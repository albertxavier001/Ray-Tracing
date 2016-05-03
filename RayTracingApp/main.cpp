#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "Scene.h"
#include "Camera.h"
#include "RayTracer.h"

#include "opencv2/core.hpp"
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/opencv.hpp"

int main()
{
#ifdef _SMALL_SCENE
	cv::Mat img(100, 100, CV_32FC3, cv::Scalar(0, 0, 0));
#else
	cv::Mat img(600, 600, CV_32FC3, cv::Scalar(0, 0, 0));
#endif // _SMALL_SCENE

	Scene scene;
	scene.loadObj("./scene\\ori\\scene_1_obj/scene01.obj", "./scene\\ori\\scene_1_obj/");
	//scene.loadObj("./scene/scene01.obj", "./scene/");
	//scene.loadObj("./scene/myscene.obj", "./scene/");

	Light li;
	
	if (0)
	{
		int si = 1;
		for (int i = 0; i < 12 ; ++i) 
		{
			Eigen::Vector3f p;
			int ind1 = scene._shapes[si].mesh.indices[i * 3];
			int ind2 = scene._shapes[si].mesh.indices[i * 3+1];
			int ind3 = scene._shapes[si].mesh.indices[i * 3+2];
			p[0] = scene._shapes[si].mesh.positions[ind1];
			p[1] = scene._shapes[si].mesh.positions[ind2];
			p[2] = scene._shapes[si].mesh.positions[ind3];
			p[3] = 1;
			li.lightPos = p;
			float intensity = 0.5;
			li.intensity[0] = intensity;
			li.intensity[1] = intensity;
			li.intensity[2] = intensity;
			li.type = 0;// point light
			scene.lights.push_back(li);
		}
		


	}
	else 
	{
		li.intensity = Eigen::Vector3f(/*"1 1 1"*/1,1,1);
		li.type = 1;// area light
		//li.lid = 1;// debug refract
		li.lid = 2;// debug all
		li.shape = &(scene._shapes[li.lid]);
	}
	scene.lights.push_back(li);

	Camera cam;

	RayTracer tracer(cam, scene);
	tracer.render(img);
	
	return 0;

}