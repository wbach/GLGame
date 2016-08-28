#ifndef GUI_SHADER_H
#define GUI_SHADER_H
#include <vector>
#include "ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

class GUIShader : public CShaderProgram{

	int location_transformationMatrix;

public:
	GUIShader(){}
	void init();
	void getAllUniformLocations();
	void bindAttributes();
	void loadTransformMatrix(glm::mat4 matrix);
	~GUIShader();

	// void loadViewMatrix(Camera camera);
};


#endif