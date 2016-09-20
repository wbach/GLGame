#include "Model.h"


int CModel::AddMesh(string name, vector<float> &positions, vector<float>&text_coords, vector<float>&normals, vector<float>&tangents,vector<unsigned int> &indices, SMaterial &material) {
	
	CMesh mesh;
	mesh.m_VertexCount = 0;
	this->m_Name = name;
	mesh.material = material;
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

	mesh.CalculateBoudnigBox(positions);

	mesh.m_Vao = Utils::CreateVao();
	GLuint vboId = Utils::BindIndicesBuffer(indices); mesh.m_Vbos.push_back(vboId);
	if (positions.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(0, 3, positions);
		mesh.m_Vbos.push_back(vboId);
	}
	if (text_coords.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(1, 2, text_coords);
		mesh.m_Vbos.push_back(vboId);
	}
	if (normals.size() > 0)
	{
		GLuint vboId = Utils::StoreDataInAttributesList(2, 3, normals);
		mesh.m_Vbos.push_back(vboId);
	}
	if (tangents.size() > 0) 
	{
		GLuint vboId = Utils::StoreDataInAttributesList(3, 3, tangents);
		mesh.m_Vbos.push_back(vboId);
	}
	Utils::UnbindVao();
	m_Meshes.push_back(mesh);
	return 0;
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
	for (unsigned int x = 0; x < m_Vbos.size(); x++) {
		glDeleteBuffers(1, &m_Vbos[x]);
	}
	glDeleteVertexArrays(1, &m_Vao);
}
