#pragma once
#include "Helper.h"
#include "library/armadillo/include/armadillo"
class Camera
{
public:
	Helper hp;

	Eigen::Vector3f ori;

	Eigen::Vector3f eye;
	Eigen::Vector3f look;
	Eigen::Vector3f up;

	Eigen::Vector3f xDir;
	Eigen::Vector3f yDir;

	float dx, dy;

	//arma::ivec2 image;
	float fov;


	int w;
	int h;

public:
	Camera();
	~Camera();



};

