#include "GUIButton.h"

 CGUIButton::CGUIButton(GLuint normalTextureID, GLuint hoverTextureID, GLuint activeTextureID, string text, glm::vec2 position, float font_size, glm::vec3 colour, glm::vec2 size)
: m_Size(size)
, m_Position(position)
, m_Colour(colour)
, m_Text(text)
, m_FontSize(font_size)
, m_State(GuiButtonState::NORMAL)
, m_NormalTexture(normalTextureID, position, size)
, m_HoverTexture(hoverTextureID, position, size)
, m_ActiveTexture(activeTextureID, position, size)
 {
	 m_ButtonTexture = m_NormalTexture;
 }
 CGUIButton::CGUIButton(string text, glm::vec2 position, float font_size, glm::vec3 colour, glm::vec2 size)
: CGUIButton(0, 0, 0, text, position, font_size, colour, size)
 {
 }
 void CGUIButton::UpdateText(string text) 
 {
	 m_Text = text;
 }

 int CGUIButton::CheckStatus(const glm::vec2& window_size)
 {
	 m_State = GuiButtonState::NORMAL;
	 int tmp_x, tmp_y;
	 SDL_GetMouseState(&tmp_x, &tmp_y);
	 float x = (((float)tmp_x) / (float)window_size.x) * 2 - 1;
	 float y = (1 - (((float)tmp_y) / (float)window_size.y)) * 2 - 1;

	 if (x >= m_Position.x - m_Size.x  && x < m_Position.x + m_Size.x) 
	 {
		 if (y >= m_Position.y - m_Size.y  && y < m_Position.y + m_Size.y) 
		 {
			 if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
			 {
				 m_State = GuiButtonState::ACTIVE;
			 }
			 m_State = GuiButtonState::HOVER;
		 }
	 }
	 return m_State;
 }

 void CGUIButton::DrawText(FontShader* shader, freetype::font_data* font)
 {
	 shader->Start();
	 glActiveTexture(GL_TEXTURE0);
	 GLfloat ActiveColor[] = { m_Colour.x, m_Colour.y, m_Colour.z, 1 };
	 glPushMatrix();
	 glLoadIdentity();
	 glColor4fv(ActiveColor);
	 shader->loadTranslation(m_Position);
	 glScalef(m_FontSize, m_FontSize, m_FontSize);
	 freetype::print(*font, m_Position.x, m_Position.y, m_Text.c_str());
	 glPopMatrix();
	 shader->Stop();
 }
