#include "SkyBoxRenderer.h"

void CSkyBoxRenderer::Init(const glm::mat4& projection_matrix, const float& distance_view)
{
	m_DistanceView = distance_view;
	m_SkyBoxShader.Init();
	m_SkyBoxShader.Start();
	m_SkyBoxShader.LoadProjectionMatrix(projection_matrix);
	m_SkyBoxShader.Stop();
}

void CSkyBoxRenderer::Render(const glm::mat4& view_matrix, const float&  delta_time, const float& blend_factor)
{
	m_SkyBoxShader.Start();
	m_SkyBoxShader.LoadViewMatrix(view_matrix, delta_time, m_DistanceView);
	float color = 0.8f * blend_factor;
	m_SkyBoxShader.LoadFogColour(color, color, color);
	glBindVertexArray(m_QubeID);
	glEnableVertexAttribArray(0);
	BindTextures(blend_factor);
	glDrawElements(GL_TRIANGLES, m_QubeVertexCount, GL_UNSIGNED_SHORT, 0);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	m_SkyBoxShader.Stop();
}
void CSkyBoxRenderer::SetTextures(GLuint day, GLuint night)
{
	m_DayTexture	= day;
	m_NightTexture	= night;
}
void CSkyBoxRenderer::SetMeshId(GLuint box_id, int veretex_count)
{
	m_QubeID = box_id;
	m_QubeVertexCount = veretex_count;
}
void CSkyBoxRenderer::CleanUp()
{
	m_SkyBoxShader.CleanUp();
}
void CSkyBoxRenderer::BindTextures(const float& blend_factor) const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_DayTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_NightTexture);
	//cout << blend_factor << endl;
	m_SkyBoxShader.LoadBlendFactor(blend_factor);
}
