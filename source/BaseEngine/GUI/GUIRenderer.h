#pragma once
#include <memory>
#include <vector>
#include <GL/glew.h>
#include "../Loader/Loader.h"
#include "GUIShader.h"
#include "GUITexture.h"
#include "GUIText.h"
#include "GUIButton.h"
#include "GUI.h"
using namespace std;
class CGUIRenderer
{	
public:
	void Init(int window_width, int window_height);
	void SetFont(const string& filename);
	void LoadCursor(shared_ptr<CGUITexture> cur);

	void Render(const SGUI &gui);
	void RenderText(const vector<CGUIText> &texts) const;
	void RenderButtons(const vector<CGUIButton> &buttons);
	void RenderTextures(const vector<CGUITexture> &guis);
	void CleanUP();
private:
	GLuint m_QuadVao, m_IndVbo, m_VertexVbo;

	GUIShader m_Shader;
	glm::mat4 m_TransformationMatrix;

	shared_ptr<CGUITexture> m_Cursor;
	glm::vec2				m_WindowSize;

	//freetype::font_data m_TextFont;
	CFont				m_TextFont;
	FontShader			m_Fontshader;
	string				m_FontName;
};
