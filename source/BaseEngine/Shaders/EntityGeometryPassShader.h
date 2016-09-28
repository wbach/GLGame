#pragma once
#include "ShaderProgram.h"

class CEntityGeometryPassShader : public CShaderProgram
{
public:
	void GetAllUniformLocations() override;
	void BindAttributes() override;

	void Init();
	void LoadTransformMatrix(const glm::mat4&) const;
	void LoadProjectionMatrix(const glm::mat4&) const;
	void LoadViewMatrix(const glm::mat4&) const;
private:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
};
