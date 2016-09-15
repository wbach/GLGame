#pragma once
#include "../Shaders/ShaderProgram.h"
#include "../Lights/Light.h"
#include "../Utils/Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Camera/Camera.h"

class FontShader : public CShaderProgram{
	int location_translation ;
	int location_transformationMatrix;
public:
	void init(){
		initShaderProgram("Data/Shaders/fontVertex.vs" ,"Data/Shaders/fontFragment.fs" ) ;
		this->start() ;
		this->getAllUniformLocations();
		glm::mat4 scaleMat = createTransformationMatrix(glm::vec2(0),glm::vec2(0.0005,0.001));
		loadTransformation(scaleMat);
	//	loadTranslation(glm::vec2(0.0,-0.95));
		this->stop();
	}
	void getAllUniformLocations(){
		location_translation = getUniformLocation("translation");
		location_transformationMatrix = getUniformLocation("transformationMatrix");
	}
	void bindAttributes() {
	//	bindAttribute(0, "position");
	//	bindAttribute(1, "textureCoords");
	}
	void loadColour(glm::vec3 colour){
	//	loadValue(location_colour, colour);
	}
	void loadTranslation(glm::vec2 pos){
		loadValue(location_translation, pos);
	}
	void loadTransformation(glm::mat4 pos){
		loadValue(location_transformationMatrix, pos);
	}
};
