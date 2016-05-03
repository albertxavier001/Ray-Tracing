#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "Scene.h"
#include "tiny_obj_loader.h"
#include <vector>
#include <iostream>

Scene::Scene()
{
}


Scene::~Scene()
{
}

void Scene::loadObj(std::string objFileName, std::string mtlFileName)
{
	std::string inputfile = objFileName;
	

	std::string err;
	bool ret = tinyobj::LoadObj(_shapes, _materials, err, inputfile.c_str(), mtlFileName.c_str());

	if (!err.empty()) { // `err` may contain warning message.
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	std::cout << "# of shapes    : " << _shapes.size() << std::endl;
	std::cout << "# of materials : " << _materials.size() << std::endl;
	float scale = 1.0;

#ifdef _SMALL_SCENE
	scale = 0.1;
#endif // _SMALL_SCENE
	for (size_t i = 0; i < _shapes.size(); i++) {
		printf("shape[%ld].name = %s\n", i, _shapes[i].name.c_str());
		printf("Size of shape[%ld].indices: %ld\n", i, _shapes[i].mesh.indices.size());
		printf("Size of shape[%ld].normals: %ld\n", i, _shapes[i].mesh.normals.size());
		printf("Size of shape[%ld].material_ids: %ld\n", i, _shapes[i].mesh.material_ids.size());
		assert((_shapes[i].mesh.indices.size() % 3) == 0);
 		for (size_t f = 0; f < _shapes[i].mesh.indices.size() / 3; f++) {
 		//	printf("  idx[%ld] = %d, %d, %d. mat_id = %d\n", f, shapes[i].mesh.indices[3 * f + 0], shapes[i].mesh.indices[3 * f + 1], shapes[i].mesh.indices[3 * f + 2], shapes[i].mesh.material_ids[f]);
			tinyobj::tag_t tag;
			tag.stringValues.push_back(_materials[_shapes[i].mesh.material_ids[f]].name);
			_shapes[i].mesh.tags.push_back(tag);
		}

		printf("shape[%ld].vertices: %ld\n", i, _shapes[i].mesh.positions.size());
		assert((_shapes[i].mesh.positions.size() % 3) == 0);
		for (size_t v = 0; v < _shapes[i].mesh.positions.size() / 3; v++) {
// 			printf("  v[%ld] = (%f, %f, %f)\n", v,
// 				shapes[i].mesh.positions[3 * v + 0],
// 				shapes[i].mesh.positions[3 * v + 1],
// 				shapes[i].mesh.positions[3 * v + 2]);
			_shapes[i].mesh.positions[3 * v + 0] *= scale;
			_shapes[i].mesh.positions[3 * v + 1] *= scale;
			_shapes[i].mesh.positions[3 * v + 2] *= scale;
			
			
		}

		

	}
	for (size_t i = 0; i < _materials.size(); i++) {
		printf("material[%ld].name = %s\n", i, _materials[i].name.c_str());
		printf("  material.Ka = (%f, %f ,%f)\n", _materials[i].ambient[0], _materials[i].ambient[1], _materials[i].ambient[2]);
		printf("  material.Kd = (%f, %f ,%f)\n", _materials[i].diffuse[0], _materials[i].diffuse[1], _materials[i].diffuse[2]);
		printf("  material.Ks = (%f, %f ,%f)\n", _materials[i].specular[0], _materials[i].specular[1], _materials[i].specular[2]);
		printf("  material.Tr = (%f, %f ,%f)\n", _materials[i].transmittance[0], _materials[i].transmittance[1], _materials[i].transmittance[2]);
		printf("  material.Ke = (%f, %f ,%f)\n", _materials[i].emission[0], _materials[i].emission[1], _materials[i].emission[2]);
		printf("  material.Ns = %f\n", _materials[i].shininess);
		printf("  material.Ni = %f\n", _materials[i].ior);
		printf("  material.dissolve = %f\n", _materials[i].dissolve);
		printf("  material.illum = %d\n", _materials[i].illum);
		printf("  material.map_Ka = %s\n", _materials[i].ambient_texname.c_str());
		printf("  material.map_Kd = %s\n", _materials[i].diffuse_texname.c_str());
		printf("  material.map_Ks = %s\n", _materials[i].specular_texname.c_str());
		printf("  material.map_Ns = %s\n", _materials[i].specular_highlight_texname.c_str());
		std::map<std::string, std::string>::const_iterator it(_materials[i].unknown_parameter.begin());
		std::map<std::string, std::string>::const_iterator itEnd(_materials[i].unknown_parameter.end());
		for (; it != itEnd; it++) {
			printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
		}
		printf("\n");
	}

	genShapse(this->_shapes, this->_materials, this->shapes);

}

void Scene::genShapse(
	const std::vector<tinyobj::shape_t>			&_shapes, 
	std::vector<tinyobj::material_t>			&_materials, 
	std::vector<struct MESH>				&shapes
	)
{
	int shapeSize = _shapes.size();
	shapes.resize(shapeSize);
	for (int si = 0; si < shapeSize ; ++si) 
	{
		int faceSize = _shapes[si].mesh.indices.size() / 3;
		shapes[si].name = _shapes[si].name;
		if (shapes[si].name.find("Sphere") != std::string::npos)
			shapes[si].type = SPHERE;
		else
			shapes[si].type = TRIANGLE;

		shapes[si].id = si;
		shapes[si].faces.resize(faceSize);
		shapes[si].bbox.xMin = INFINITY;
		shapes[si].bbox.xMax = -INFINITY;
		shapes[si].bbox.yMin = INFINITY;
		shapes[si].bbox.yMax = -INFINITY;
		shapes[si].bbox.zMin = INFINITY;
		shapes[si].bbox.zMax = -INFINITY;
		
		for (int f = 0; f < faceSize; ++f)
		{
			
			for (int v = 0; v < 3 ; ++v) 
			{

				for (int co = 0; co < 3 ; ++co) 
				{
					/// geometry
					int vIdx = _shapes[si].mesh.indices[f * 3 + v];
					shapes[si].faces[f].geo.v[v][co] = _shapes[si].mesh.positions[vIdx * 3 + co];
					shapes[si].faces[f].geo.n[v][co] = _shapes[si].mesh.normals[vIdx * 3 + co];
				}
				float eps = 1e-1;
				/// update bounding box
				if (shapes[si].bbox.xMin > shapes[si].faces[f].geo.v[v][0]) shapes[si].bbox.xMin = shapes[si].faces[f].geo.v[v][0] - eps;
				if (shapes[si].bbox.yMin > shapes[si].faces[f].geo.v[v][1]) shapes[si].bbox.yMin = shapes[si].faces[f].geo.v[v][1] - eps;
				if (shapes[si].bbox.zMin > shapes[si].faces[f].geo.v[v][2]) shapes[si].bbox.zMin = shapes[si].faces[f].geo.v[v][2] - eps;
				if (shapes[si].bbox.xMax < shapes[si].faces[f].geo.v[v][0]) shapes[si].bbox.xMax = shapes[si].faces[f].geo.v[v][0] + eps;
				if (shapes[si].bbox.yMax < shapes[si].faces[f].geo.v[v][1]) shapes[si].bbox.yMax = shapes[si].faces[f].geo.v[v][1] + eps;
				if (shapes[si].bbox.zMax < shapes[si].faces[f].geo.v[v][2]) shapes[si].bbox.zMax = shapes[si].faces[f].geo.v[v][2] + eps;
			}

			/// material
			int mtlIdx = _shapes[si].mesh.material_ids[f];

			shapes[si].faces[f].mtl.ambient = Eigen::Vector3f(
				_materials[mtlIdx].ambient[0],
				_materials[mtlIdx].ambient[1],
				_materials[mtlIdx].ambient[2]);

			shapes[si].faces[f].mtl.diffuse = Eigen::Vector3f(
				_materials[mtlIdx].diffuse[0],
				_materials[mtlIdx].diffuse[1],
				_materials[mtlIdx].diffuse[2]);

			shapes[si].faces[f].mtl.specular = Eigen::Vector3f(
				_materials[mtlIdx].specular[0],
				_materials[mtlIdx].specular[1],
				_materials[mtlIdx].specular[2]);

			std::stringstream ss;
			ss << _materials[mtlIdx].unknown_parameter["Tf"];
			float tf1, tf2, tf3;
			ss >> tf1 >> tf2 >> tf3;
			Eigen::Vector3f refract(1 - tf1, 1 - tf2, 1 - tf3);
			shapes[si].faces[f].mtl.refract = refract;
			
			shapes[si].faces[f].mtl.shininess = _materials[mtlIdx].shininess;
			shapes[si].faces[f].mtl.Ni = _materials[mtlIdx].ior;
			shapes[si].faces[f].mtl.type = LAMBERTIAN;
			if (_materials[mtlIdx].name.find("blinn") != std::string::npos)
				shapes[si].faces[f].mtl.type = PHONG;

		}
		
		if (shapes[si].type == SPHERE)
		{
			shapes[si].center = shapes[si].getCenter(shapes[si].faces);
			shapes[si].radius = shapes[si].getRadius(shapes[si].faces, shapes[si].center);
			printf("shape[%d] center = (%f,%f,%f) radius=%f\n", si, shapes[si].center.x(),
				shapes[si].center.y(), shapes[si].center.z(), shapes[si].radius);
		}

	}

	for (int i = 0; i < shapes.size() ; ++i) 
	{
		if (shapes[i].type == SPHERE) printf("s %d type SPHERE\n", i);
		if (shapes[i].type == TRIANGLE) printf("s %d type TRI\n", i);
	}
	printf("End load scene\n");
}
