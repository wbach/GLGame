#pragma once
#include <math.h>
#include <GL/glew.h>
#include "../Loader/Model.h"
#include "../Shaders/SkyBoxShader.h"

class CSkyBoxRenderer 
{
public:
	void Init(const glm::mat4& projection_matrix, const float& distance_view);
	void Render(const glm::mat4& view_matrix, const float& delta_time, const float& blend_factor);
	void SetTextures(GLuint day, GLuint night);
	void SetMeshId(GLuint box_id, int veretex_count);
	void CleanUp();
private:
	void BindTextures(const float& blend_factor) const;
	float m_DistanceView = 100;
	CSkyBoxShader m_SkyBoxShader;

	GLuint m_DayTexture;
	GLuint m_NightTexture;
	
	GLuint	m_QubeID;
	int		m_QubeVertexCount;
};