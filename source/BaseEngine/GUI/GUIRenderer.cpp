#include "GUIRenderer.h"

void CGUIRenderer::Init(int window_width, int window_height) 
{
	vector<float> postions = {
		-1,1,
		-1,-1,
		1,1,
		1,-1 };
	m_QuadVao = Utils::CreateVao();
	m_VertexVbo = Utils::StoreDataInAttributesList(0, 2, postions);
	Utils::UnbindVao();

	m_Shader.Init();
	m_Fontshader.Init();
	m_TextFont.init("Data/GUI/CRYSISB.ttf", 50);
	//textFont.init("CRYSISB.ttf", 50);//"bgothm.ttf"
	m_Cursor = nullptr;
	m_WindowSize.x = window_width;
	m_WindowSize.y = window_height;
}

void CGUIRenderer::SetFont(const string & filename)
{
	m_TextFont.clean();
	m_TextFont.init(filename.c_str(), 50);
}

void CGUIRenderer::LoadCursor(shared_ptr<CGUITexture> cur)
{
	m_Cursor = cur;
}

void CGUIRenderer::Render(const SGUI & gui)
{
	RenderTextures(gui.guiTextures);
	RenderButtons(gui.guiButtons);
	RenderText(gui.guiTexts);
}

void CGUIRenderer::RenderText(const vector<CGUIText>& texts) const
{
	for (const CGUIText& text : texts) 
	{
		text.DrawText(m_Fontshader, m_TextFont);
	}
}

void CGUIRenderer::RenderButtons(const vector<CGUIButton>& buttons)
{
	if (buttons.size() <= 0) return;
	
	m_Shader.Start();
	glBindVertexArray(m_QuadVao);
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	for (const CGUIButton& button : buttons)
	{
		if (!button.m_ButtonTexture.m_IsLoaded) continue;

		glActiveTexture(GL_TEXTURE0);
		switch (button.m_State)
		{
		case GuiButtonState::NORMAL:  glBindTexture(GL_TEXTURE_2D, button.m_NormalTexture.GetTextureId()); break;
		case GuiButtonState::ACTIVE:  glBindTexture(GL_TEXTURE_2D, button.m_ActiveTexture.GetTextureId());   break;
		case GuiButtonState::HOVER:  glBindTexture(GL_TEXTURE_2D, button.m_HoverTexture.GetTextureId());  break;
		}

		m_TransformationMatrix = Utils::CreateTransformationMatrix(button.m_Position, button.m_Size);
		m_Shader.LoadTransformMatrix(m_TransformationMatrix);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_Shader.Stop();
}

void CGUIRenderer::RenderTextures(const vector<CGUITexture>& guis)
{
	m_Shader.Start();
	glBindVertexArray(m_QuadVao);
	glEnableVertexAttribArray(0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	for (const CGUITexture& gui : guis) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gui.GetTextureId());
		m_TransformationMatrix = Utils::CreateTransformationMatrix(gui.GetPosition(), gui.GetScale());
		m_Shader.LoadTransformMatrix(m_TransformationMatrix);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_Shader.Stop();

	if (m_Cursor != nullptr)
	{
		m_Shader.Start();
		glBindVertexArray(m_QuadVao);
		glEnableVertexAttribArray(0);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		int tmpx, tmpy;
		SDL_GetMouseState(&tmpx, &tmpy);
		float x = (((float)tmpx) / (float)m_WindowSize.x) * 2 - 1;
		float y = (1 - (((float)tmpy) / (float)m_WindowSize.y)) * 2 - 1;
		m_Cursor->SetPosition(glm::vec2(x + m_Cursor->GetScale().x / 2, y - m_Cursor->GetScale().y / 2));
		SDL_ShowCursor(SDL_DISABLE);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_Cursor->GetTextureId());
		m_TransformationMatrix = Utils::CreateTransformationMatrix(m_Cursor->GetPosition(), m_Cursor->GetScale());
		m_Shader.LoadTransformMatrix(m_TransformationMatrix);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		m_Shader.Stop();
	}
}

void CGUIRenderer::CleanUP() {
	m_Shader.CleanUp();
	m_Fontshader.CleanUp();
	glDeleteBuffers(1, &m_VertexVbo);
	glDeleteBuffers(1, &m_IndVbo);
	glDeleteVertexArrays(1, &m_QuadVao);
	m_TextFont.clean();
}
