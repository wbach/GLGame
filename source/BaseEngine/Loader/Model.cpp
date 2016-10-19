#include "Model.h"


CModel::CModel()
	: m_InstancedRendering(false)
	, m_Name("No name model")
	, m_LoaderType(LoaderType::NONE)
	, m_BoundingBoxMin(0)
	, m_BoundingBoxMax(0)
	, m_BoundingCenter(0)
	, m_BoundingSize(0)
{
}

void CModel::CreateTransformsVbo(std::vector<glm::mat4>& m)
{
	for (CMesh& mesh : m_Meshes)
	{
		mesh.CreateTransformsVbo(m);
	}
	m_InstancedRendering = true;
}

CMesh* CModel::AddMesh(string name, vector<float> &positions, vector<float>&text_coords, vector<float>&normals, vector<float>&tangents,vector<unsigned int> &indices, SMaterial &material)
{
	CMesh mesh;
	// Normalize to define scale (height) 1 unit = 1 metr
	mesh.CalculateBoudnigBox(positions);
	mesh.CreateFaces();
	m_Name = name;
	mesh.m_Material = material;	
	mesh.m_VertexCount = indices.size();
	mesh.positions = positions;
	mesh.normals = normals;
	mesh.indices = indices;
	mesh.tangents = tangents;
	mesh.text_coords = text_coords;
	mesh.CreateVaoMesh();
	m_Meshes.push_back(mesh);
	return &m_Meshes[m_Meshes.size()-1];
}
void CModel::CreateMeshesVaos()
{
	for (CMesh& mesh : m_Meshes)
		mesh.CreateVaoMesh();
}
void CModel::CalculateBoudnigBox()
{
	bool first = true;
	for (CMesh& mesh : m_Meshes)
	{
		if (first)
		{
			m_BoundingBoxMin = mesh.m_BoundingBoxMin;
			m_BoundingBoxMax = mesh.m_BoundingBoxMax;
			first = false;
		}
		else
		{
			m_BoundingBoxMin = Utils::CalculateMinimumVector(m_BoundingBoxMin, mesh.m_BoundingBoxMin);
			m_BoundingBoxMax = Utils::CalculateMinimumVector(mesh.m_BoundingBoxMax, m_BoundingBoxMax);
			m_BoundingBoxMax = Utils::CalculateMinimumVector(m_BoundingBoxMax, mesh.m_BoundingBoxMax);
		}
	}
	m_BoundingSize = m_BoundingBoxMax - m_BoundingBoxMin;
	m_BoundingCenter = (m_BoundingBoxMax + m_BoundingBoxMin) / 2.f;
}
glm::mat4 CModel::CalculateNormalizedMatrix(float w, float h, float z)
{
	int axis;
	if (w == 0 && h == 0 && z == 0)
		return glm::mat4(1);
	else if (w != 0 && h == 0 && z == 0)
		axis = 1;
	else if (w == 0 && h != 0 && z == 0)
		axis = 2;
	else if (w == 0 && h == 0 && z != 0)
		axis = 3;

	CalculateBoudnigBox();

	glm::vec3 scale_vector;
	switch (axis)
	{
	case 1:
		scale_vector = glm::vec3(w / m_BoundingSize.x, w / m_BoundingSize.x, w / m_BoundingSize.x);
		break;
	case 2: scale_vector = glm::vec3(h / m_BoundingSize.y, h / m_BoundingSize.y, h / m_BoundingSize.y);
		break;
	case 3: scale_vector = glm::vec3(z / m_BoundingSize.z, z / m_BoundingSize.z, z / m_BoundingSize.z);
		break;
	default: scale_vector = glm::vec3(w / m_BoundingSize.x, h / m_BoundingSize.y, z / m_BoundingSize.z);
		break;
	}
	glm::mat4 normalized_matrix = Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), scale_vector);	
	return normalized_matrix;
}
const string & CModel::GetName() const
{
	return m_Name;
}
const vector<CMesh>& CModel::GetMeshes() const
{
	return m_Meshes;
}
float CModel::GetBoundingMaxSize()
{
	float max = FLT_MIN;
	for (CMesh& mesh : m_Meshes)
	{
		if (max < mesh.GetBoundingSize().x)
			max = mesh.GetBoundingSize().x;
		if (max < mesh.GetBoundingSize().y)
			max = mesh.GetBoundingSize().y;
		if (max < mesh.GetBoundingSize().z)
			max = mesh.GetBoundingSize().z;
	}

	return max;
}
void CModel::CleanUp()
{
	for (CMesh& mesh : m_Meshes)
	{
		mesh.CleanUp();
	}
	m_Meshes.clear();
}
CMesh::CMesh() {}
CMesh::CMesh(SMaterial material)
: m_Material(material)
{
}
void CMesh::CalculateBoudnigBox(const vector<float>& positions )
{
	Utils::CalculateBoudnigBox(positions, m_BoundingBoxMin, m_BoundingBoxMax, m_BoundingSize, m_BoundingCenter);
}

void CMesh::CleanUp()
{	
	glDeleteBuffers(VertexBufferObjects::COUNT, m_Vbos);	
	glDeleteVertexArrays(1, &m_Vao);
}

void CMesh::UpdateVertexPosition(const vector<float>& vertices) const
{	
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbos[VertexBufferObjects::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
}

void CMesh::CreateVaoMesh()
{
	m_Vao = Utils::CreateVao();
	GLuint vboId = Utils::BindIndicesBuffer(indices); m_Vbos[VertexBufferObjects::INDICES] = vboId;
	if (positions.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(0, 3, positions);
		m_Vbos[VertexBufferObjects::POSITION] = vboId;
	}
	if (text_coords.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(1, 2, text_coords);
		m_Vbos[VertexBufferObjects::TEXT_COORD] = vboId;
	}
	if (normals.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(2, 3, normals);
		m_Vbos[VertexBufferObjects::NORMAL] = vboId;
	}
	if (tangents.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(3, 3, tangents);
		m_Vbos[VertexBufferObjects::TANGENT] = vboId;
	}
	Utils::UnbindVao();
}

void CMesh::CreateTransformsVbo(std::vector<glm::mat4>& m)
{
	glBindVertexArray(m_Vao);
	glGenBuffers(1, &m_Vbos[VertexBufferObjects::TRANSFORM_MATRIX]);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbos[VertexBufferObjects::TRANSFORM_MATRIX]);	
	for (unsigned int i = 0; i < 4; i++) 
	{	
		glEnableVertexAttribArray(4 + i);
		glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (const GLvoid*)(sizeof(GLfloat) * i * 4));
		glVertexAttribDivisor(4 + i, 1);			
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m.size(), &m[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CMesh::CreateFaces()
{
	m_Faces.clear();
	vector<glm::vec3> norm, vertexes;
	for (unsigned int x = 0; x < positions.size(); x += 3)
	{
		float& xx = positions[x];
		float& yy = positions[x + 1];
		float& zz = positions[x + 2];
		vertexes.push_back(glm::vec3(xx, yy, zz));
	}
	for (unsigned int x = 0; x < normals.size(); x += 3)
	{
		norm.push_back(glm::vec3(normals[x], normals[x + 1], normals[x + 2]));
	}
	for (unsigned int x = 0; x < indices.size(); x += 3)
	{
		SFace face;
		face.vertex[0] = vertexes[indices[x]];
		face.vertex[1] = vertexes[indices[x + 1]];
		face.vertex[2] = vertexes[indices[x + 2]];
		//face.normal = norm[this->indices[x]] ;

		float Vector1[3];
		float Vector2[3];

		// wyznaczanie wektorow....
		Vector1[0] = face.vertex[0].x - face.vertex[1].x;
		Vector1[1] = face.vertex[0].y - face.vertex[1].y;
		Vector1[2] = face.vertex[0].z - face.vertex[1].z;

		Vector2[0] = face.vertex[1].x - face.vertex[2].x;
		Vector2[1] = face.vertex[1].y - face.vertex[2].y;
		Vector2[2] = face.vertex[1].z - face.vertex[2].z;

		//wyznaczanie normalnych....
		face.normal.x = Vector1[1] * Vector2[2] - Vector1[2] * Vector2[1];
		face.normal.y = Vector1[2] * Vector2[0] - Vector1[0] * Vector2[2];
		face.normal.z = Vector1[0] * Vector2[1] - Vector1[1] * Vector2[0];

		face.normal = glm::normalize(face.normal);

		m_Faces.push_back(face);
	}
}

const GLuint& CMesh::GetVao() const
{
	return m_Vao;
}

const GLuint & CMesh::GetVbo(VertexBufferObjects::Type type) const
{
	return m_Vbos[type];
}

const vector<SFace>& CMesh::GetFaces() const
{
	return m_Faces;
}

const SMaterial & CMesh::GetMaterial() const
{
	return m_Material;
}

const glm::vec3 & CMesh::GetBoundingSize()
{
	return m_BoundingSize;
}

const glm::vec3 & CMesh::GetBoundingMin()
{
	return m_BoundingBoxMin;
}

const glm::vec3 & CMesh::GetBoundingMax()
{
	return m_BoundingBoxMax;
}

const glm::vec3 & CMesh::GetBoundingCenter()
{
	return m_BoundingCenter;
}

const GLuint& CMesh::GetVertexCount() const
{
	return m_VertexCount;
}
