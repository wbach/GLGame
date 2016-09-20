#include "GUIText.h"

CGUIText::CGUIText(string text, glm::vec2 position, float font_size, glm::vec3 colour)
: m_Position(position)
, m_Text(text)
, m_Colour(colour)
, m_FontSize(font_size)
{
}

void CGUIText::updateText(string text)
{
	m_Text = text;
}

void CGUIText::DrawText(const FontShader & shader, const freetype::font_data & font) const
{
	shader.Start();
	glActiveTexture(GL_TEXTURE0);
	GLfloat active_color[] = { m_Colour.x, m_Colour.y, m_Colour.z, 1 };
	glPushMatrix();
	glLoadIdentity();
	glColor4fv(active_color);
	shader.loadTranslation(m_Position);
	glScalef(m_FontSize, m_FontSize, m_FontSize);
	freetype::print(font, m_Position.x, m_Position.y, m_Text.c_str());
	glPopMatrix();
	shader.Stop();
}
