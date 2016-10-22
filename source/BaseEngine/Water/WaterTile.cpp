#include "WaterTile.h"

CWaterTile::CWaterTile()
{
}

CWaterTile::CWaterTile(glm::vec3 position, float size, float wave_speed, GLuint dudv_texture, GLuint normal_texture, glm::vec4 color)
{
	Init(position, size, wave_speed, dudv_texture, normal_texture, color);
}

void CWaterTile::Init(glm::vec3 position, float size, float wave_speed, GLuint dudv_texture, GLuint normal_texture, glm::vec4 color)
{
	m_WaveSpeed = wave_speed;
	m_Position = position;
	m_Size = size;
	m_DudvTexture = dudv_texture;
	m_NormalMap = normal_texture;
	Utils::CreateQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord, m_QuadIndicesSize);
	m_TransformMatrix = Utils::CreateTransformationMatrix(m_Position, glm::vec3(-90, 0, 0), glm::vec3(m_Size));
	m_Color = color; 
}

void CWaterTile::Update(const float& delta_time)
{
	m_MoveFactor += m_WaveSpeed*delta_time;
	m_MoveFactor = fmod(m_MoveFactor, 1);
}

const GLuint & CWaterTile::GetDudvTexture() const
{
	return m_DudvTexture;
}

const GLuint & CWaterTile::GetNormalTexture() const
{
	return m_NormalMap;
}

const glm::vec4 & CWaterTile::GetColor() const
{
	return m_Color;
}

const glm::vec3& CWaterTile::GetPosition() const
{
	return m_Position;
}

const float& CWaterTile::GetSize() const
{
	return m_Size;
}

const float& CWaterTile::GetWaveSpeed() const
{
	return m_WaveSpeed;
}

const float & CWaterTile::GetMoveFactor() const
{
	return m_MoveFactor;
}

const glm::mat4 & CWaterTile::GetTransformMatrix() const
{
	return m_TransformMatrix;
}

const int & CWaterTile::GetIndicesSize() const
{
	return m_QuadIndicesSize;
}

const GLuint & CWaterTile::GetMeshVao() const
{
	return m_QuadVao;
}

void CWaterTile::CleanUp()
{
	Utils::DeleteQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord);
	glDeleteTextures(1, &m_NormalMap);
	glDeleteTextures(1, &m_DudvTexture);
}
