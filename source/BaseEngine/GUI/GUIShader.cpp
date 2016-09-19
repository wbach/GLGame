#include "GUIShader.h"

void GUIShader::Init()
{
	InitShaderProgram("Data/Shaders/guiVertexShader.vs" ,"Data/Shaders/guiFragmentShader.fs" ) ;
	Start() ;
	GetAllUniformLocations();
	Stop();
}

void GUIShader::GetAllUniformLocations()
{
	location_transformationMatrix = GetUniformLocation("transformationMatrix");
}

void GUIShader::BindAttributes()
{
	BindAttribute(0, "position");
}

void GUIShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_transformationMatrix, matrix);
}

GUIShader::~GUIShader()
{

}
