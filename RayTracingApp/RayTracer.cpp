#include "RayTracer.h"


RayTracer::RayTracer(Camera &cam, Scene &scene)
	:cam(cam),
	scene(scene),
#if SMALL_SCENE
	maxSpecularDepth(5)

#else
	maxSpecularDepth(5)
#endif
{
	scene.gIntersector = &gIntersctor;
}


RayTracer::~RayTracer()
{
}

float randomf(float LO, float HI)
{
	float r3 = LO + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
	return r3;
}

Eigen::Vector3f RayTracer::getSample(int x, int y)
{
	int nsample = 50;
	Eigen::Vector3f c(0, 0, 0);
	std::vector<std::vector<Eigen::Vector3f> > cc(nsample, std::vector<Eigen::Vector3f>(nsample, Eigen::Vector3f(/*"0 0 0"*/0,0,0)));
	int i, j;
#ifdef NDEBUG
#pragma omp parallel for private(j)
#endif // NDEBUG
	for (i = 0; i < nsample; i++) {
		for (j = 0; j < nsample; j++) {
			float small = 1. / 2 / nsample;
			Eigen::Vector2f p((float)x, (float)y), offset(i *1.f / nsample + randomf(-small, small), j *1.f / nsample + randomf(-small, small));
			Ray ray = genRay(p, offset); // generate ray for each sample
			cc[i][j] = traceRay(ray, scene);
		}
	}
	for (int i = 0; i < nsample; i++) {
		for (int j = 0; j < nsample; j++) {
			c += cc[i][j] ;
		}
	}
	Eigen::Vector3f newC = c / (float)(nsample*nsample);
	//newC.print("newC");
	return newC;
}

Ray RayTracer::genRay(Eigen::Vector2f p, Eigen::Vector2f offset)
{
	Eigen::Vector3f to = cam.ori + 
		  cam.xDir * cam.dx * (p[0]+offset[0]) 
		+ cam.yDir * cam.dy * (p[1]+offset[1]);
	return Ray(cam.eye, (to-cam.eye).normalized());
}

int i, j;

Eigen::Vector3f CosWeightedRandomHemisphereDirection21(const Eigen::Vector3f &_n)
{

	
	float Xi1 = (float)rand() / (float)RAND_MAX;
	float Xi2 = (float)rand() / (float)RAND_MAX;

	float  theta = acos(sqrt(1.0 - Xi1));
	float  phi = 2.0 * M_PI * Xi2;

	float xs = sinf(theta) * cosf(phi);
	float ys = cosf(theta);
	float zs = sinf(theta) * sinf(phi);

	Eigen::Vector3f y = _n;
	Eigen::Vector3f h = y;
	if (fabs(h[0]) <= fabs(h[1]) && fabs(h[0]) <= fabs(h[2]))
		h[0] = 1.0;
	else if (fabs(h[1]) <= fabs(h[0]) && fabs(h[1]) <= fabs(h[2]))
		h[1] = 1.0;
	else
		h[2] = 1.0;


	Eigen::Vector3f x = h.cross(y).normalized();
	Eigen::Vector3f z = x.cross(y).normalized();

	Eigen::Vector3f direction = xs * x + ys * y + zs * z;
	return direction.normalized();
}


Eigen::Vector3f uniformSampleHemisphere(const float &r1, const float &r2)
{
	float sinTheta = sqrtf(1 - r1*r1);
	float phi = 2 * M_PI * r2;
	float x = sinTheta * cosf(phi);
	float z = sinTheta * sinf(phi);
	Eigen::Vector3f ret(x,r1,z);
	return /*arma::normalise*/(ret).normalized();
}


Eigen::Vector3f transCoordinate(const Eigen::Vector3f& normal, const Eigen::Vector3f& dir)
{
	

	Eigen::Vector3f Nt, Nb;
	if (std::fabs((float)normal[0]) > std::fabs((float)normal[1]))
	{
		Nt[0] = normal[2];
		Nt[1] = 0;
		Nt[2] = -normal[0];
		Nt = /*arma::normalise*/(Nt.normalized());
	}
	else
	{
		Nt[0] = 0;
		Nt[1] = -normal[2];
		Nt[2] = -normal[1];
		Nt = /*arma::normalise*/(Nt.normalized());
	}
	Nb = /*arma::cross*/(normal.cross(Nt));
	Eigen::Matrix3f trans_mat;
	trans_mat.col(0) = Nt;
	trans_mat.col(1) = normal;
	trans_mat.col(2) = Nb;
	Eigen::Vector3f tmp = trans_mat*dir;
	tmp.normalize();
	return tmp;
}


Eigen::Vector3f RayTracer::traceRay(Ray &ray, Scene &scene)
{
	
	Intersector intersector;
	Eigen::Vector3f rgb(/*"0,0,0"*/0,0,0);
	if (gIntersctor.intersectScene(ray, intersector, scene.shapes))
	{
		/// intersect light source
		if (intersector.id == scene.lights[0].lid)
		{
			rgb += intersector.areaLight*50;
			return rgb;
		}

		/*if (intersector.id == 0) return Eigen::Vector3f(100, 0, 0);
		if (intersector.id == 1) return Eigen::Vector3f(0, 100, 0);
		if (intersector.id == 2) return Eigen::Vector3f(0, 0, 100);
		if (intersector.id == 3) return Eigen::Vector3f(100, 0, 100);*/

		// recursive trace ray
		if (ray.depth < maxSpecularDepth)
		{
			
			if (intersector.bsdf.refract[0]<1e-6)
			{
				//reflect 
				Eigen::Vector3f wi;
				Eigen::Vector3f specularBSDF;
				Ray newray;
				
				if (intersector.id != 1) // lambertian
				{
					float r1 = randomf(0, 1);
					float r2 = randomf(0, 1);
					Eigen::Vector3f nor = intersector.bsdf.nn;
					if (intersector.id == 0) nor = -nor; // Wall: normal is inversed
					wi = transCoordinate(nor, uniformSampleHemisphere(r1, r2));
					newray = Ray(intersector.p + 1e-3*wi, wi, ray, intersector.re);
					specularBSDF = intersector.bsdf.f(ray.dir*(-1.f), newray.dir);
				}
				else
				{
					Eigen::Vector3f nor = intersector.bsdf.nn;
					if (intersector.id == 0) nor = -intersector.bsdf.nn;
					float proj_len = /*arma::dot*/nor.dot(ray.dir*(-1.f));
					wi = nor * 2 * proj_len - /*hp.normalise4v*/(ray.dir*(-1.f)).normalized();
					newray = Ray(intersector.p + 1e-3*wi, wi, ray, intersector.re);
					specularBSDF = intersector.bsdf.specular;
				}
				//if (intersector.bsdf.refract[0] > 1e-6) specularBSDF *= 0.2;
				Eigen::Vector3f curLi = traceRay(newray, scene);
				rgb += (curLi.array() * specularBSDF.array()).matrix();

			}
			//else
			if (intersector.bsdf.refract[0] > 1e-6)
			{
				//refract
				Eigen::Vector3f wt;
				Eigen::Vector3f refractBSDF = intersector.bsdf.sample(ray.dir*(-1.f), wt, BSDF::REFRACT);
				if (refractBSDF[0] != 0 || refractBSDF[1] != 0 || refractBSDF[2] != 0)
				{
					Ray newray = Ray(intersector.p + 1e-3*wt, wt, ray, intersector.re);
					Eigen::Vector3f curLi = traceRay(newray, scene);
					rgb += (curLi.array() * refractBSDF.array()).matrix();
				}
			}
		}

	}
	return rgb;
}

void RayTracer::render(cv::Mat &dst)
{
	// for each pixel

	for (i = 0; i < dst.rows; ++i)
	{
		for (j = 0; j <dst.cols ; ++j)
		{
#ifdef _DEBUG
			i = 63;
			j = 60;
#endif // _DEBUG
			Eigen::Vector3f color = getSample(j, i);
			dst.at<cv::Vec3f>(dst.rows - 1 - i, dst.cols - 1 - j) = cv::Vec3f(pow(color[2], 0.4545f)*255.0f + 0.5f, pow(color[1], 0.4545f)*255.0f + 0.5f, pow(color[0], 0.4545f)*255.0f + 0.5f);
			//dst.at<cv::Vec3f>(dst.rows-1-i, dst.cols-1-j) = cv::Vec3f(color[2], color[1], color[0]);
		}
	}
	cv::imwrite("res.png", dst);
}