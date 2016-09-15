#include "Model.h"


int CModel::addMesh(string name, vector<float> &positions, vector<float>&textCoords, vector<float>&normals, vector<float>&tangents,vector<unsigned int> &indices, Material &material) {
	
	Mesh mesh;
	mesh.vertexCount = 0;
	this->name = name;
	mesh.material = material;
	vector<glm::vec3> norm, vertexes;
	for (unsigned int x = 0; x < positions.size(); x += 3) {
		vertexes.push_back(glm::vec3(positions[x], positions[x + 1], positions[x + 2]));
		mesh.vertexCount++;
	}
	for (unsigned int x = 0; x < normals.size(); x += 3) {
		norm.push_back(glm::vec3(normals[x], normals[x + 1], normals[x + 2]));
	}
	for (unsigned int x = 0; x < indices.size(); x += 3) {
		Face face;
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

		mesh.faces.push_back(face);
	}

	mesh.calculateBoudnigBox(positions);

	mesh.vao = createVAO();
	GLuint vboId = bindIndicesBuffer(indices); mesh.vbos.push_back(vboId);
	if (positions.size() >0) {
		GLuint vboId = storeDataInAttributesList(0, 3, positions);
		mesh.vbos.push_back(vboId);
	}
	if (textCoords.size() >0) {
		GLuint vboId = storeDataInAttributesList(1, 2, textCoords);
		mesh.vbos.push_back(vboId);
	}
	if (normals.size() >0) {
		GLuint vboId = storeDataInAttributesList(2, 3, normals);
		mesh.vbos.push_back(vboId);
	}
	if (tangents.size() >0) {
		GLuint vboId = storeDataInAttributesList(3, 3, tangents);
		mesh.vbos.push_back(vboId);
	}
	unbindVAO();
	meshes.push_back(mesh);
	return 0;
}
void Mesh::calculateBoudnigBox(vector<float>& positions)
{
	if (positions.size() == 0)
		return;

	boundingBoxMin.x = boundingBoxMax.x = positions[0];
	boundingBoxMin.y = boundingBoxMax.y = positions[1];
	boundingBoxMin.z = boundingBoxMax.z = positions[2];
	for (unsigned int i = 0; i < positions.size(); i += 3) {
		if (positions[i] < boundingBoxMin.x) boundingBoxMin.x = positions[i];
		if (positions[i] > boundingBoxMax.x) boundingBoxMax.x = positions[i];
		if (positions[i + 1] < boundingBoxMin.y) boundingBoxMin.y = positions[i + 1];
		if (positions[i + 1] > boundingBoxMax.y) boundingBoxMax.y = positions[i + 1];
		if (positions[i + 2] < boundingBoxMin.z) boundingBoxMin.z = positions[i + 2];
		if (positions[i + 2] > boundingBoxMax.z) boundingBoxMax.z = positions[i + 2];
	}
	boundingSize = glm::vec3(boundingBoxMax.x - boundingBoxMin.x, boundingBoxMax.y - boundingBoxMin.y, boundingBoxMax.z - boundingBoxMin.z);
	boundingCenter = glm::vec3((boundingBoxMin.x + boundingBoxMax.x) / 2, (boundingBoxMin.y + boundingBoxMax.y) / 2, (boundingBoxMin.z + boundingBoxMax.z) / 2);
}
