#pragma once
#include "../Shaders/ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

class CShadowShader : public CShaderProgram
{	
public:
	void Init();
	void GetAllUniformLocations();
	void BindAttributes() override;
	void LoadTransformMatrix(const glm::mat4& matrix) const;
	void LoadProjectionMatrix(const glm::mat4& matrix) const;
	void LoadViewMatrix(const glm::mat4& matrix) const;

	void LoadUseInstancedRendering(const float& use) const;

private:
	int location_TransformationMatrix;
	int location_ProjectionMatrix;
	int location_ViewMatrix;

	int location_IsInstancedRender;
};