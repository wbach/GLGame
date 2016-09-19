#ifndef GUITEXT_H
#define GUITEXT_H
#include "GUITexture.h"
#include "FreeType.h"
#include "FontShader.h"
class CGUIText{


	float fontSize ;
public:
    string text;
	glm::vec2 position ;
	glm::vec3 colour;
	CGUIText() {}
	CGUIText(string text, glm::vec2 position, float fontSize,glm::vec3 colour){
		this->position = position;
		this->colour = colour;
		this->text = text ;
		this->fontSize = fontSize ;
	}
	void updateText(string text){this->text = text;}
	void drawText(FontShader *shader,freetype::font_data *font) const
	{
		//glUseProgram(0);
		shader->start();
		glActiveTexture(GL_TEXTURE0) ;
		GLfloat ActiveColor[]={colour.x,colour.y,colour.z,1};
		glPushMatrix();
		glLoadIdentity();
		glColor4fv(ActiveColor);
		shader->loadTranslation(position);
		glScalef(fontSize,fontSize,fontSize);
		freetype::print(*font,position.x, position.y,text.c_str());
		glPopMatrix() ;
		shader->stop();
	}

};
#endif
