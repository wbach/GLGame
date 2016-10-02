#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "../Utils/Utils.h"
#include "Material.h"
using namespace std;
namespace VertexBufferObjects
{
	enum Type
	{
		INDICES = 0,
		POSITION,
		TEXT_COORD,
		NORMAL,
		TANGENT,
		COUNT,
	};
};
namespace LoaderType
{
	enum Type
	{
		ASSIMP = 0,
		FBX,
		NONE,
		COUNT
	};
}
struct SFace
{
	glm::vec3 vertex[3];
	glm::vec3 normal;
	glm::vec2 uv[3];
	glm::vec3 center;
	float skipDistance;
};
class CModel;
class CMesh
{
public:
	CMesh();
	CMesh(SMaterial material);
	void CalculateBoudnigBox(vector<float>& positions);	
	void UpdateVertexPosition(const vector<float>& vertices) const;

	const GLuint& GetVao() const;
	const GLuint& GetVbo(VertexBufferObjects::Type type) const;	
	const GLuint& GetVertexCount() const;
	const vector<SFace>& GetFaces() const;
	const SMaterial& GetMaterial() const;

	void CleanUp();
private:
	glm::vec3	m_BoundingBoxMin, 
				m_BoundingBoxMax,
				m_BoundingCenter,
				m_BoundingSize;
	
	SMaterial	m_Material;

	GLuint			m_Vao;
	GLuint			m_Vbos[VertexBufferObjects::COUNT];
	vector<SFace>	m_Faces;
	GLuint			m_VertexCount;
	friend class CModel;
};
class CModel
{	
public:
	virtual void InitModel(string file_name) = 0;
	virtual void Update(float dt) {};
	CMesh* AddMesh(string name, vector<float>& positions, vector<float>& text_coords, vector<float>& normals, vector<float>& tangents,
		vector<unsigned int>& indices, SMaterial& material);
	const	string& GetName() const;
	const	vector<CMesh>& GetMeshes() const;
	virtual void CleanUp();
protected:
	string			 m_Name;
	vector<CMesh>	 m_Meshes;
	LoaderType::Type m_LoaderType;
};

