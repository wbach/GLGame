#pragma once
#include "../Shaders/ShaderProgram.h"
#include "../Lights/Light.h"
#include "../Utils/Utils.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Camera/Camera.h"

class FontShader : public CShaderProgram
{
public:
	void Init(){
		InitShaderProgram("Data/Shaders/fontVertex.vs" ,"Data/Shaders/fontFragment.fs" ) ;
		Start() ;
		GetAllUniformLocations();
		glm::mat4 scaleMat = Utils::CreateTransformationMatrix(glm::vec2(0),glm::vec2(0.0005,0.001));
		loadTransformation(scaleMat);
	//	loadTranslation(glm::vec2(0.0,-0.95));
		Stop();
	}
	void GetAllUniformLocations() override
	{
		location_translation = GetUniformLocation("translation");
		location_transformationMatrix = GetUniformLocation("transformationMatrix");
	}
	void BindAttributes() override 
	{
	//	bindAttribute(0, "position");
	//	bindAttribute(1, "textureCoords");
	}
	void loadColour(glm::vec3 colour){
	//	loadValue(location_colour, colour);
	}
	void loadTranslation(glm::vec2 pos){
		LoadValue(location_translation, pos);
	}
	void loadTransformation(glm::mat4 pos){
		LoadValue(location_transformationMatrix, pos);
	}
private:
	int location_translation;
	int location_transformationMatrix;
};
