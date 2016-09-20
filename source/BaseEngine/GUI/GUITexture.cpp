#include "GUITexture.h"

CGUITexture::CGUITexture()
: m_IsLoaded(false)
{ 
}

CGUITexture::CGUITexture(GLuint textureID, glm::vec2 position, glm::vec2 scale)
: m_TextureId(textureID)
, m_Position(position)
, m_Scale(scale)
, m_IsLoaded(true)
{
}

const glm::vec2 & CGUITexture::GetPosition() const
{ 
	return m_Position; 
}

const glm::vec2 & CGUITexture::GetScale() const 
{ 
	return m_Scale;
}

const int & CGUITexture::GetTextureId() const 
{ 
	return m_TextureId;
}

void CGUITexture::SetTexture(const int& id) 
{
	m_TextureId = id; 
}

void CGUITexture::SetPosition(const glm::vec2 & position) 
{ 
	m_Position = position; 
}

void CGUITexture::SetXScale(const float & x)
{
	m_Scale.x = x;
}

void CGUITexture::SetXposition(const float & x)
{
	m_Position.x = x;
}

void CGUITexture::SetScale(const glm::vec2& scale)
{ 
	m_Scale = scale; 
}
