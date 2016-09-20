#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "../Utils/Utils.h"
#include "Material.h"
using namespace std;
struct SFace
{
	glm::vec3 vertex[3];
	glm::vec3 normal;
	glm::vec2 uv[3];
	glm::vec3 center;
	float skipDistance;
};
class CMesh
{
public:
	GLuint m_Vao;
	vector<GLuint> m_Vbos;
	vector<SFace> m_Faces;
	int m_VertexCount;
	glm::vec3 m_BoundingBoxMin, m_BoundingBoxMax, m_BoundingCenter, m_BoundingSize;
	SMaterial material;

	CMesh() {}
	CMesh(SMaterial material)
	:material(material)
	{
	}

	void CalculateBoudnigBox(vector<float>& positions);
	void CleanUp();
};
class CModel
{	
public:
	vector<CMesh> m_Meshes;

	int AddMesh(string name,vector<float> &positions, vector<float>&text_coords, vector<float>&normals, vector<float>&tangents,
		 vector<unsigned int> &indices, SMaterial &material);

	const string& GetName() const
	{ 
		return m_Name;
	}
	void CleanUp()
	{
		for (CMesh mesh : m_Meshes) {
			mesh.CleanUp();
		}
	}
private:
	string m_Name;
};
#pragma once
