#pragma once
#include "GUITexture.h"
#include "FreeType.h"
#include "FontShader.h"
class CGUIText
{	
public:
    string text;
	glm::vec2 position ;
	glm::vec3 colour;
	CGUIText() {}
	CGUIText(string text, glm::vec2 position, float fontSize,glm::vec3 colour){
		this->position = position;
		this->colour = colour;
		this->text = text ;
		this->m_FontSize = fontSize ;
	}
	void updateText(string text){this->text = text;}
	void drawText(FontShader *shader,freetype::font_data *font) const
	{
		//glUseProgram(0);
		shader->Start();
		glActiveTexture(GL_TEXTURE0) ;
		GLfloat ActiveColor[]={colour.x,colour.y,colour.z,1};
		glPushMatrix();
		glLoadIdentity();
		glColor4fv(ActiveColor);
		shader->loadTranslation(position);
		glScalef(m_FontSize, m_FontSize, m_FontSize);
		freetype::print(*font,position.x, position.y,text.c_str());
		glPopMatrix() ;
		shader->Stop();
	}
private:
	float m_FontSize;

};
