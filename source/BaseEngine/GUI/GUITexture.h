#pragma once
#include <gl/glew.h>
#include "glm/gtc/matrix_transform.hpp"
class CGUITexture
{	
public:
	bool m_IsLoaded;
	
	CGUITexture();
	CGUITexture(GLuint textureID, glm::vec2 position, glm::vec2 scale);
	
	const glm::vec2& GetPosition() const;
	const glm::vec2& GetScale() const;
	const int& GetTextureId() const;
	
	void SetTexture(const int& id);
	void SetPosition(const glm::vec2& position);
	void SetXScale(const float& x);
	void SetXposition(const float& x);
	void SetScale(const glm::vec2& scale);
	//void CleanUp(){
	//	//usuwanie w loader
	//	//glDeleteTextures(1,&textureID);
	//}
private:
	GLuint m_TextureId;
	glm::vec2 m_Position;
	glm::vec2 m_Scale;
};
