#include "SkyBoxRenderer.h"

void CSkyBoxRenderer::Init(const glm::mat4& projection_matrix, const float& distance_view)
{
	m_DistanceView = distance_view;
	m_SkyBoxShader.Init();
	m_SkyBoxShader.Start();
	m_SkyBoxShader.LoadProjectionMatrix(projection_matrix);
	m_SkyBoxShader.Stop();
}

void CSkyBoxRenderer::Render(const glm::mat4& view_matrix, const float&  delta_time)
{
	m_SkyBoxShader.Start();
	m_SkyBoxShader.LoadViewMatrix(view_matrix, delta_time, m_DistanceView);
	m_SkyBoxShader.LoadFogColour(.8f, .8f, .8f);
	m_SkyBoxShader.LoadBlendFactor(0.f);

	glBindVertexArray(m_QubeID);
	glEnableVertexAttribArray(0);
	BindTextures(delta_time);
	glDrawElements(GL_TRIANGLES, m_QubeVertexCount, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, m_QubeVertexCount);
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
float CSkyBoxRenderer::BindTextures(const float& delta_time) const
{
	/*time += delta_time * 100;
	time = fmod(time, 24000);
	int texture1;
	int texture2;
	float blendFactor;
	if (time >= 0 && time < 5000) 
	{
		texture1 = nightTexture;
		texture2 = nightTexture;
		blendFactor = (time - 0) / (5000 - 0);
	}
	else if (time >= 5000 && time < 8000)
	{
		texture1 = nightTexture;
		texture2 = texture;
		blendFactor = (time - 5000) / (8000 - 5000);
	}
	else if (time >= 8000 && time < 21000) 
	{
		texture1 = texture;
		texture2 = texture;
		blendFactor = (time - 8000) / (21000 - 8000);
	}
	else {
		texture1 = texture;
		texture2 = nightTexture;
		blendFactor = (time - 21000) / (24000 - 21000);
	}*/

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_DayTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_NightTexture);

	return 1;
}
