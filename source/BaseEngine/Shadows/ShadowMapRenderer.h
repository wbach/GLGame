#pragma once
#include "ShadowShader.h"
#include "ShadowBox.h"
#include "ShadowFrameBuffer.h"
#include "../Engine/Scene.h"

class CShadowMapRenderer 
{
public:
	CShadowMapRenderer();
	void Init(shared_ptr<CCamera>& camera, glm::vec2 window_size, int fov, int near_plane);
	void Render(shared_ptr<CScene>& scene);
	void SetShadowMapSize(float size);
	void CleanUp();
	const GLuint& GetShadowMap() const;
	const glm::mat4& GetToShadowMapSpaceMatrix() const;
private:
	void RenderEntity(const shared_ptr<CEntity>&, CModel& model) const;
	void Prepare(const glm::vec3& light_direction, const CShadowBox &box);
	void Finish();
	void UpdateLightViewMatrix(glm::vec3 direction, glm::vec3 center);
	void UpdateOrthoProjectionMatrix(float width, float height, float length);

	glm::mat4 m_ProjectonMatrix;
	glm::mat4 m_LightViewMatrix;
	glm::mat4 m_ProjectionViewMatrix;

	CShadowShader		m_ShadowShader;
	CShadowFrameBuffer	m_ShadowFbo;
	CShadowBox			m_ShadowBox;

	glm::mat4 m_Offset;

	float	m_ShadowMapSize;
};