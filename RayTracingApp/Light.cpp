#include "Light.h"
#include "Intersector.h"
#include "Ray.h"

Light::Light()
	:
	intensity()
{
}


Light::~Light()
{
}

Eigen::Vector3f Light::sampleLight(Scene& scene, Intersector &gIntersctor, Eigen::Vector3f p, Eigen::Vector3f& wi, float rayEpsilon) const
{
	//printf("samplelight\n");
	if (type == 0)
	{
		//printf("light type = 0\n");
		Eigen::Vector3f li = intensity;
		Intersector inter;
		Eigen::Vector3f dir = lightPos - p;
		Ray ray = Ray(p, dir, rayEpsilon, /*arma::norm*/(lightPos - p).norm());

		while (1)
		{
			if (gIntersctor.intersectScene(ray, inter, scene.shapes))
			{
				if (inter.bsdf.refract[0] == 0 &&
					inter.bsdf.refract[1] == 0 &&
					inter.bsdf.refract[2] == 0)
				{
					//printf("refract");
					return Eigen::Vector3f(/*"0 0 0"*/0,0,0);
				}
				else
				{
					//li %= inter.bsdf.refract;
					li = li.array() * inter.bsdf.refract.array();
					ray = Ray(inter.p, lightPos - inter.p, inter.re, /*arma::norm*/(lightPos - inter.p).norm());
				}

			}
			else
			{
				Eigen::Vector3f v = lightPos - p;
				v /= /*arma::norm*/(v.norm());
				wi = v;
				return li;
			}
		}
	}
	else if (type == 1) // area light
	{

		Eigen::Vector3f li(/*"0 0 0"*/0,0,0);
		int nsample = 0;

		for (int i = 0; i </* shape->mesh.indices.size() / 3*/1; ++i)
		{
			const int nsampler = 2;

			int ind1, ind2, ind3;
			Eigen::Vector3f p1, p2, p3;
			int x = i;
			ind1 = shape->mesh.indices[3 * x + 0];
			ind2 = shape->mesh.indices[3 * x + 1];
			ind3 = shape->mesh.indices[3 * x + 2];

			float yoffset = -0.3 * shape->mesh.positions[3 * ind1 + 1];

			p1[0] = shape->mesh.positions[3 * ind1 + 0];
			p1[1] = shape->mesh.positions[3 * ind1 + 1] + yoffset;
			p1[2] = shape->mesh.positions[3 * ind1 + 2];
			p1[3] = 1;

			p2[0] = shape->mesh.positions[3 * ind2 + 0];
			p2[1] = shape->mesh.positions[3 * ind2 + 1] + yoffset;
			p2[2] = shape->mesh.positions[3 * ind2 + 2];
			p2[3] = 1;

			p3[0] = shape->mesh.positions[3 * ind3 + 0];
			p3[1] = shape->mesh.positions[3 * ind3 + 1] + yoffset;
			p3[2] = shape->mesh.positions[3 * ind3 + 2];
			p3[3] = 1;

			Eigen::Vector3f v1 = p2 - p1;
			Eigen::Vector3f v2 = p3 - p1;

			//Eigen::Vector3f li;
			for (int j = 0; j < nsampler; j++)
			{
				for (int k = 0; k < nsampler; k++)
				{
					//printf("trace area light (%d,%d)\n", j, k);
					float t1 = 1.f / (float)nsampler * j;
					float t2 = 1.f / (float)nsampler * k;

					if ((t1 + t2) >= 1.f) {
						continue;
					}
					Eigen::Vector3f thisli = intensity;
					nsample++;

					Eigen::Vector3f s = p1 + v1*t1 + v2*t2;

					Ray ray(p, (s - p), rayEpsilon, /*arma::norm*/(s - p).norm());

					Intersector inter;
					int lastid = -1;
					int lastlastid = -1;

					while (true) {
						if (scene.gIntersector->intersectScene(ray, inter, scene.shapes) && inter.id != lastlastid && inter.id != lid/*shape->mesh.tags[0].intValues[0]*/) {
							lastid = inter.id;
							lastlastid = lastid;
							//break;
							//System.out.println(nsample);
							if (inter.bsdf.refract[0] ==
								inter.bsdf.refract[1] ==
								inter.bsdf.refract[2] == 0) {
								break;
							}
							else { // 
								//thisli %= inter.bsdf.refract * 0.8;
								thisli = thisli.array() * inter.bsdf.refract.array() * 0.8;
								ray = Ray(inter.p, s - inter.p,
									inter.re, /*arma::norm*/(s - inter.p).norm());
							}
						}
						else { // direct
							li += thisli;
							/*li.print("li");
							thisli.print("thisli");
							intensity.print("intensity");*/
							break;
						}
					}
				}
			}


			if (li[0] == 0. &&  li[1] == 0. && li[2] == 0)
			{
				//return li;
				continue;
			}
			else
			{
				Helper hp;
				wi = /*hp.normalise4v*/((p1 + p2 + p3) / 3 - p).normalized();
				//li.print("li");
				//return li;
				continue;
			}
		}
		li /= nsample;
		return li;
	}
}
