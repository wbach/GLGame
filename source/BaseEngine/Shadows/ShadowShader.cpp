#include "ShadowShader.h"

void CShadowShader::Init()
{
	InitShaderProgram("Data/Shaders/Shadows/ShadowVertexShader.vert", "Data/Shaders/Shadows/ShadowFragmentShader.frag");
	Start();
	GetAllUniformLocations();
	Stop();
}

void CShadowShader::GetAllUniformLocations()
{
	location_TransformationMatrix = GetUniformLocation("TransformationMatrix");
	location_ProjectionMatrix = GetUniformLocation("ProjectionMatrix");
	location_ViewMatrix = GetUniformLocation("ViewMatrix");
}

void CShadowShader::BindAttributes()
{
	BindAttribute(0, "Position");
	BindAttribute(1, "TexCoord");
}

void CShadowShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_TransformationMatrix, matrix);
}

void CShadowShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ProjectionMatrix, matrix);
}

void CShadowShader::LoadViewMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_ViewMatrix, matrix);
}
