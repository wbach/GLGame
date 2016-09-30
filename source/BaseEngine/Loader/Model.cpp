#include "Model.h"


int CModel::AddMesh(string name, vector<float> &positions, vector<float>&text_coords, vector<float>&normals, vector<float>&tangents,vector<unsigned int> &indices, SMaterial &material) {
	
	CMesh mesh;
	
	this->m_Name = name;
	mesh.m_Material = material;
	vector<glm::vec3> norm, vertexes;
	for (unsigned int x = 0; x < positions.size(); x += 3) 
	{
		vertexes.push_back(glm::vec3(positions[x], positions[x + 1], positions[x + 2]));
		mesh.m_VertexCount++;
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

		mesh.m_Faces.push_back(face);
	}	
	
	mesh.m_VertexCount = indices.size();

	mesh.CalculateBoudnigBox(positions);

	mesh.m_Vao = Utils::CreateVao();
	GLuint vboId = Utils::BindIndicesBuffer(indices); mesh.m_Vbos[VertexBufferObjects::INDICES] = vboId;
	if (positions.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(0, 3, positions);
		mesh.m_Vbos[VertexBufferObjects::POSITION] = vboId;
	}
	if (text_coords.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(1, 2, text_coords);
		mesh.m_Vbos[VertexBufferObjects::TEXT_COORD] = vboId;
	}
	if (normals.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(2, 3, normals);
		mesh.m_Vbos[VertexBufferObjects::NORMAL] = vboId;
	}
	if (tangents.size() > 0) 
	{
		GLuint vboId = Utils::StoreDataInAttributesList(3, 3, tangents);
		mesh.m_Vbos[VertexBufferObjects::TANGENT] = vboId;
	}
	Utils::UnbindVao();
	m_Meshes.push_back(mesh);
	return 0;
}
const string & CModel::GetName() const
{
	return m_Name;
}
const vector<CMesh>& CModel::GetMeshes() const
{
	return m_Meshes;
}
void CModel::CleanUp()
{
	for (CMesh& mesh : m_Meshes)
	{
		mesh.CleanUp();
	}
}
CMesh::CMesh() {}
CMesh::CMesh(SMaterial material)
: m_Material(material)
{
}
void CMesh::CalculateBoudnigBox(vector<float>& positions)
{
	if (positions.size() == 0)
		return;

	m_BoundingBoxMin.x = m_BoundingBoxMax.x = positions[0];
	m_BoundingBoxMin.y = m_BoundingBoxMax.y = positions[1];
	m_BoundingBoxMin.z = m_BoundingBoxMax.z = positions[2];
	for (unsigned int i = 0; i < positions.size(); i += 3) {
		if (positions[i] < m_BoundingBoxMin.x) m_BoundingBoxMin.x = positions[i];
		if (positions[i] > m_BoundingBoxMax.x) m_BoundingBoxMax.x = positions[i];
		if (positions[i + 1] < m_BoundingBoxMin.y) m_BoundingBoxMin.y = positions[i + 1];
		if (positions[i + 1] > m_BoundingBoxMax.y) m_BoundingBoxMax.y = positions[i + 1];
		if (positions[i + 2] < m_BoundingBoxMin.z) m_BoundingBoxMin.z = positions[i + 2];
		if (positions[i + 2] > m_BoundingBoxMax.z) m_BoundingBoxMax.z = positions[i + 2];
	}
	m_BoundingSize = glm::vec3(m_BoundingBoxMax.x - m_BoundingBoxMin.x, m_BoundingBoxMax.y - m_BoundingBoxMin.y, m_BoundingBoxMax.z - m_BoundingBoxMin.z);
	m_BoundingCenter = glm::vec3((m_BoundingBoxMin.x + m_BoundingBoxMax.x) / 2, (m_BoundingBoxMin.y + m_BoundingBoxMax.y) / 2, (m_BoundingBoxMin.z + m_BoundingBoxMax.z) / 2);
}

void CMesh::CleanUp()
{
	for (unsigned int x = 0; x < VertexBufferObjects::COUNT; x++)
	{
		glDeleteBuffers(1, &m_Vbos[x]);
	}
	glDeleteVertexArrays(1, &m_Vao);
}

void CMesh::UpdateVertexPosition(const vector<glm::vec3>& vertices) const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbos[VertexBufferObjects::POSITION]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
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

const GLuint& CMesh::GetVertexCount() const
{
	return m_VertexCount;
}
