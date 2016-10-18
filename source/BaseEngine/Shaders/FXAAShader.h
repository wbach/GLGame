#pragma once

#include "ShaderProgram.h"

class CFXAAShader : public CShaderProgram
{
public:
	void Init();
	void LoadTransformMatrix(const glm::mat4&) const;
	void LoadProjectionMatrix(const glm::mat4&) const;
	void LoadViewMatrix(const glm::mat4&) const;

	void ConnectTextureUnits() const;

	void GetAllUniformLocations() override;
	void BindAttributes() override;
private:
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;

	int location_Texture;
};