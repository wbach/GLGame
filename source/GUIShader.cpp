#include "GUIShader.h"

void GUIShader::init()
{
	initShaderProgram("Data/Shaders/guiVertexShader.vs" ,"Data/Shaders/guiFragmentShader.fs" ) ;
	this->start() ;
	this->getAllUniformLocations();
	this->stop();
}

void GUIShader::getAllUniformLocations()
{
	location_transformationMatrix = getUniformLocation("transformationMatrix");
}

void GUIShader::bindAttributes()
{
	bindAttribute(0, "position");
}

void GUIShader::loadTransformMatrix(glm::mat4 matrix)
{
	loadValue(location_transformationMatrix, matrix);
}

GUIShader::~GUIShader()
{

}
