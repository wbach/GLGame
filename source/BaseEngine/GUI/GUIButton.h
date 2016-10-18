#pragma once
#include "GUITexture.h"
#include "Font.h"
#include "FontShader.h"
#include "../Input/InputManager.h"
namespace GuiButtonState 
{
	enum {
		NORMAL = 0,
		HOVER,
		ACTIVE
	};
}

class CGUIButton 
{
public:
	CGUITexture m_ButtonTexture;
	CGUITexture m_NormalTexture;
	CGUITexture m_HoverTexture;
	CGUITexture m_ActiveTexture;
	int m_State;
	glm::vec2 m_Position, m_Size;
	glm::vec3 m_Colour;

	CGUIButton() {}
	CGUIButton(CInputManager* input_manager, string text, glm::vec2 position, float fontSize, glm::vec3 colour, glm::vec2 size);
	CGUIButton(CInputManager* input_manager, GLuint normal_texture_id, GLuint hover_texture_id, GLuint active_texture_id, string text, glm::vec2 position, float font_size, glm::vec3 colour, glm::vec2 size);
	void UpdateText(string text);
	int CheckStatus(const glm::vec2& window_size);

	void DrawText(const FontShader *shader, const CFont& font) const;
private:
	string m_Text;
	float m_FontSize;
	CInputManager* m_InputManager;
};