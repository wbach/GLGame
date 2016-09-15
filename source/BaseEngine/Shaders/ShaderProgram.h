#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"
using namespace std;
class CShaderProgram {
	int programID; 
	int vertexShaderID;
	int fragmentShaderID;
	int tesselationControlShaderID;
	int tesselationEvaluationShaderID;
	void loadFile(const char* fn, std::string& str);
	unsigned int loadShader(char *filename, unsigned int mode);
public:
	void initShaderProgram(char* vertexShaderFile, char* fragmentShaderFile);
	void initShaderProgram(char* vertexShaderFile, char* fragmentShaderFile,char *tesselationShaderFile, char * tesselationEvaluationShaderFile);
	void start();
	void stop();
	void cleanUp();
	int getUniformLocation(string uniformName);
	void bindAttribute(int attribute, string variableName);
	void loadValue(unsigned int loacation, glm::mat4 value);
	void loadValue(unsigned int loacation, glm::mat3 value);
	void loadValue(unsigned int loacation, float value);
	void loadValue(unsigned int loacation, int value);
	void loadValue(unsigned int loacation, glm::vec2 value);
	void loadValue(unsigned int loacation, glm::vec3 value);
	void loadValue(unsigned int loacation, glm::vec4 value);
protected:
	virtual void getAllUniformLocations() {}
	virtual void bindAttributes() {}


};

#endif
