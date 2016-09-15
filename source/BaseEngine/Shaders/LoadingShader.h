#pragma once
#include <vector>
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Camera/Camera.h"
#include "../Lights/Light.h"
class LoadingShader : public CShaderProgram {

	int location_isTextured;
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_modelTexture;
	int location_alpha;
public:
	LoadingShader() {}
	void init();
	void loadIsTextured(float isTex);
	void getAllUniformLocations();
	void bindAttributes();
	void loadTransformMatrix(glm::mat4 matrix);
	void loadProjectionMatrix(glm::mat4 matrix);
	void loadViewMatrix(glm::mat4 matrix);
	void loadAlphaValue(float alpha);
	void connectTextureUnits();

	~LoadingShader();

	// void loadViewMatrix(Camera camera);
};