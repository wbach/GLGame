#include "LoadingShader.h"

void LoadingShader::init() {
	initShaderProgram("Data/Shaders/loadingShader.vs", "Data/Shaders/loadingShader.fs");
	start();
	getAllUniformLocations();
	connectTextureUnits();
	stop();
	//
}
void LoadingShader::getAllUniformLocations() {
	location_isTextured = getUniformLocation("isTextured");
	location_transformationMatrix = getUniformLocation("transformationMatrix");
	location_projectionMatrix = getUniformLocation("projectionMatrix");
	location_viewMatrix = getUniformLocation("viewMatrix");
	location_modelTexture = getUniformLocation("modelTexture");
	location_alpha = getUniformLocation("alphaBlend");

}
void LoadingShader::loadIsTextured(float isTex) {
	loadValue(location_isTextured, isTex);

}

void LoadingShader::connectTextureUnits() {
	loadValue(location_modelTexture, 0);
}
void LoadingShader::bindAttributes() {
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
}
void LoadingShader::loadTransformMatrix(glm::mat4 matrix) {
	loadValue(location_transformationMatrix, matrix);
}
void LoadingShader::loadProjectionMatrix(glm::mat4 matrix) {
	loadValue(location_projectionMatrix, matrix);
}
void LoadingShader::loadViewMatrix(glm::mat4 matrix) {
	loadValue(location_viewMatrix, matrix);
}
void LoadingShader::loadAlphaValue(float alpha)
{
	loadValue(location_alpha, alpha);
}
LoadingShader::~LoadingShader() {
}
