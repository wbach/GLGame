#include "Grass.h"

void CGrass::Init(const std::vector<glm::vec3>& positions, const GLuint& texture)
{
	m_IsInitialized = true;

	m_GrassTexture = texture;

	m_MeshVao = Utils::CreateVao();

	std::vector<float> grassvertex;
	for(const glm::vec3& position : positions)
		{
			grassvertex.push_back(position.x);
			grassvertex.push_back(position.y);
			grassvertex.push_back(position.z);
		}
	m_VertexSize = positions.size();
	m_VertexVbo = Utils::StoreDataInAttributesList(0, 3, grassvertex);
	Utils::UnbindVao();
}

void CGrass::Render() const
{
	if (!m_IsInitialized)
		return;

	Utils::DisableCulling();
	//scene->GetLoader().LoadTexture("Data/Textures/G3_Nature_Plant_Grass_06_Diffuse_01.png")
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_GrassTexture);
	glBindVertexArray(m_MeshVao);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_POINTS, 0, m_VertexSize);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	Utils::EnableCulling();
}

void CGrass::CleanUp()
{
	if (!m_IsInitialized)
		return;

	glDeleteBuffers(1, &m_VertexVbo);
	glDeleteVertexArrays(1, &m_MeshVao);

	m_IsInitialized = false;
}
