#pragma once
#include "../Utils/Utils.h"

class CGrass
{
public:
	void Init(const std::vector<glm::vec3>& positions, const GLuint& texture);
	void Render() const;
	void CleanUp();

	float  m_ViewDistance = 70.f;
private:
	GLuint m_MeshVao;
	GLuint m_VertexVbo;
	GLuint m_GrassTexture;	

	int m_VertexSize;

	bool m_IsInitialized = false;
};