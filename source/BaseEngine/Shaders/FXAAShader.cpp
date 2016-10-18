#include "FXAAShader.h"

void CFXAAShader::Init()
{
	InitShaderProgram("Data/Shaders/Game/loadingShader.vert", "Data/Shaders/Game/loadingShader.frag");
	Start();
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}
void CFXAAShader::LoadTransformMatrix(const glm::mat4 &) const
{
}
void CFXAAShader::LoadProjectionMatrix(const glm::mat4 &) const
{
}
void CFXAAShader::LoadViewMatrix(const glm::mat4 &) const
{
}
void CFXAAShader::ConnectTextureUnits() const
{
}
void CFXAAShader::GetAllUniformLocations()
{
	location_transformationMatrix = GetUniformLocation("transformationMatrix");
	location_projectionMatrix = GetUniformLocation("projectionMatrix");
	location_viewMatrix = GetUniformLocation("viewMatrix");
	location_Texture = GetUniformLocation("modelTexture");
}
void CFXAAShader::BindAttributes()
{
	BindAttribute(0, "position");
	BindAttribute(1, "textureCoords");
}
