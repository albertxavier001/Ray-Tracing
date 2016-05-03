#include "Camera.h"


Camera::Camera()
#ifdef _SMALL_SCENE
	:
	w(100),
	h(100),
	eye(/*arma::fvec4("0 0.5 2 1")*/Eigen::Vector3f(0,0.5,2)),
	look(/*arma::fvec4("0 0.5 0 1")*/Eigen::Vector3f(0,0.5,0)),
#else
	:
	w(600),
	h(600),
	eye(/*arma::fvec4("0 5 20 1")*/Eigen::Vector3f(0,5,20)),
	look(/*arma::fvec4("0 5 0 1")*/Eigen::Vector3f(0,5,0)),
#endif // _SMALL_SCENE

	
	up(/*arma::fvec4("0 1 0 0")*/Eigen::Vector3f(0, 1, 0)),
	fov(45.f)

{
	Eigen::Vector3f dir = eye - look;
	yDir = /*hp.normalise4v*/(up.normalized());
	xDir = dir.cross(yDir).normalized();/*-hp.normalise4v(hp.cross4v(dir, yDir));*/
	
	//xDir.print("xDir");
	//yDir.print("yDir");
	
	float xRange = dir.norm() * tan(fov * M_PI / 180. / 2);
	float yRange = xRange / w * h;

	std::cout << "xr = " << xRange << std::endl;
	std::cout << "yr = " << yRange << std::endl;

	dx = xRange / w * 2;
	dy = yRange / h * 2;

	std::cout << "dx = " << dx << std::endl;
	std::cout << "dy = " << dy << std::endl;

	ori = look - xRange * xDir - yRange * yDir;
	//ori.print("ori");
}


Camera::~Camera()
{
}
