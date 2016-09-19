#ifndef GUI_TEXTURE_H
#define GUI_TEXTURE_H
#include "glm/gtc/matrix_transform.hpp"
class CGUITexture{
	GLuint textureID;	
public:
	bool loaded;
	glm::vec2 position ;
	glm::vec2 scale ;
	CGUITexture() { loaded = false; }
	CGUITexture(GLuint textureID, glm::vec2 position, glm::vec2 scale) :textureID(textureID), position(position), scale(scale) { loaded = true; }
	const glm::vec2& getPosition() const {return position;}
	const glm::vec2& getScale() const {return scale;}
	const int getTextureId() const {return textureID;}
	void setTexture(int id){textureID = id ;}
	void setPosition(glm::vec2 position){this->position = position;}
	void setXScale(float x){
		scale.x = x ;
	}
	void setXposition(float x){
		position.x = x ;
	}
	void setScale(glm::vec2 scale){this->scale = scale;}
	void cleanUp(){
		//usuwanie w loader
		//glDeleteTextures(1,&textureID);
	}
};
#endif