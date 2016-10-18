#pragma once
#include "GUITexture.h"
#include "Font.h"
#include "FontShader.h"
class CGUIText
{	
public:
	CGUIText(string text, glm::vec2 position, float font_size, glm::vec3 colour);
	void updateText(string text);
	void DrawText(const FontShader &shader, const CFont& font) const;
private:
	float m_FontSize;
	string m_Text;
	glm::vec2 m_Position;
	glm::vec3 m_Colour;
};
