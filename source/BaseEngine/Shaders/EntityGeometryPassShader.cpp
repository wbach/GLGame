#include "EntityGeometryPassShader.h"

void CEntityGeometryPassShader::Init()
{
	InitShaderProgram("Data/Shaders/EntityGeometryPassShader.vert", "Data/Shaders/EntityGeometryPassShader.frag");
	Start();
	GetAllUniformLocations();
	Stop();
}
void CEntityGeometryPassShader::GetAllUniformLocations()
{
	location_transformationMatrix = GetUniformLocation("TransformationMatrix");
	location_projectionMatrix = GetUniformLocation("ProjectionMatrix");
	location_viewMatrix = GetUniformLocation("ViewMatrix");
}

void CEntityGeometryPassShader::BindAttributes()
{
	BindAttribute(0, "Position");
	BindAttribute(1, "TexCoord");
	BindAttribute(2, "Normal");
	BindAttribute(3, "Tangent");
}
void CEntityGeometryPassShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_transformationMatrix, matrix);
}

void CEntityGeometryPassShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_projectionMatrix, matrix);
}

void CEntityGeometryPassShader::LoadViewMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_viewMatrix, matrix);
}
