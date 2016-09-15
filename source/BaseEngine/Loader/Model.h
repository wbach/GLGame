#ifndef MODEL_H
#define MODEL_H
#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "../Utils/Utils.h"
#include "Material.h"
using namespace std;
struct Face {
	glm::vec3 vertex[3];
	glm::vec3 normal;
	glm::vec2 uv[3];
	glm::vec3 center;
	float skipDistance;
};
struct Mesh {
	GLuint vao;
	vector<GLuint> vbos;
	vector<Face> faces;
	int vertexCount;
	glm::vec3 boundingBoxMin, boundingBoxMax, boundingCenter, boundingSize;
	Material material;
	Mesh() {}
	Mesh(Material material) :material(material){}
	void calculateBoudnigBox(vector<float>& positions);
	void cleanUp() {

		//for (unsigned int x = 0; x < material.textures.size(); x++) {
		//	glDeleteTextures(1, &material.textures[x].id);
		//}
		for (unsigned int x = 0; x < vbos.size(); x++) {
			glDeleteBuffers(1, &vbos[x]);
		}
		glDeleteVertexArrays(1, &vao);

	};
};
class CModel {
	string name;
public:
	vector<Mesh> meshes;

	int addMesh(string name,vector<float> &positions, vector<float>&textCoords, vector<float>&normals, vector<float>&tangents,
		 vector<unsigned int> &indices, Material &material);

	string getName() { return name; }
	void cleanUp() {
		for (Mesh mesh : meshes) {
			mesh.cleanUp();
		}
	}

};
#endif // !MODEL_H


#pragma once
