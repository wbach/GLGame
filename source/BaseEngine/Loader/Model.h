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
		TRANSFORM_MATRIX,
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
	void CalculateBoudnigBox(const vector<float>& positions);
	void UpdateVertexPosition(const vector<float>& vertices) const;	
	void CreateVaoMesh();
	void CreateTransformsVbo(std::vector<glm::mat4>& m);
	void CreateFaces();

	const GLuint& GetVao() const;
	const GLuint& GetVbo(VertexBufferObjects::Type type) const;	
	const GLuint& GetVertexCount() const;
	const vector<SFace>& GetFaces() const;
	const SMaterial& GetMaterial() const;

	const glm::vec3& GetBoundingSize();
	const glm::vec3& GetBoundingMin();
	const glm::vec3& GetBoundingMax();
	const glm::vec3& GetBoundingCenter();
	void CleanUp();
private:
	//Bounding for single meshes in model
	glm::vec3	m_BoundingBoxMin, 
				m_BoundingBoxMax,
				m_BoundingCenter,
				m_BoundingSize;
	
	SMaterial	m_Material;

	GLuint			m_Vao;
	GLuint			m_Vbos[VertexBufferObjects::COUNT];
	vector<SFace>	m_Faces;
	GLuint			m_VertexCount;

	vector<float> positions;
	vector<float> text_coords;
	vector<float> normals;
	vector<float> tangents;
	vector<unsigned short> indices;

	friend class CModel;
};
class CModel
{	
public:
	CModel();
	virtual void InitModel(string file_name) = 0;
	virtual void Update(float dt) {};
	void CreateTransformsVbo(std::vector<glm::mat4>& m);

	CMesh* AddMesh(string name, vector<float>& positions, vector<float>& text_coords, vector<float>& normals, vector<float>& tangents,
		vector<unsigned short>& indices, SMaterial& material);
	
	void		SetUseInstancedRendering(const bool& s) { m_InstancedRendering = s; }
	const bool&	UseInstacedRendering() const { return m_InstancedRendering; }
	void	CreateMeshesVaos();
	void	CalculateBoudnigBox();

	const	string& GetName() const;
	const	vector<CMesh>& GetMeshes() const;	
	float	GetBoundingMaxSize();	

	virtual void CleanUp();

	glm::mat4 CalculateNormalizedMatrix(float w, float h, float z);
	bool m_UseFakeLight;
protected:
	string			 m_Name;
	vector<CMesh>	 m_Meshes;
	LoaderType::Type m_LoaderType;
	
	

	bool		m_InstancedRendering;
	//Bounding for all meshes in model
	glm::vec3	m_BoundingBoxMin,
				m_BoundingBoxMax,
				m_BoundingCenter,
				m_BoundingSize;
};

