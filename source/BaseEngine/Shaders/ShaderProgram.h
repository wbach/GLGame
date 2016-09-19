#pragma once
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
public:
	void InitShaderProgram(char* vertex_shader_file, char* fragment_shader_file);
	void InitShaderProgram(char* vertex_shader_file, char* fragment_shader_file, char *tesselation_shader_file, char * tesselation_evaluation_shader_file);
	void Start();
	void Stop();
	void CleanUp();	
	void BindAttribute(int attribute, const string& variableName) const;
	int GetUniformLocation(const string& uniformName) const;

	void LoadValue(unsigned int loacation, const int& value) const;
	void LoadValue(unsigned int loacation, const float& value) const;
	void LoadValue(unsigned int loacation, const glm::vec2& value) const;
	void LoadValue(unsigned int loacation, const glm::vec3& value) const;
	void LoadValue(unsigned int loacation, const glm::vec4& value) const;
	void LoadValue(unsigned int loacation, const glm::mat3& value) const;
	void LoadValue(unsigned int loacation, const glm::mat4& value) const;

protected:
	virtual void GetAllUniformLocations() {}
	virtual void BindAttributes() {}

private:
	void LoadFile(const char* fn, std::string& str);
	unsigned int LoadShader(char *filename, unsigned int mode);

	int m_ProgramID;
	int m_VertexShaderID;
	int m_FragmentShaderID;
	int m_TesselationControlShaderID;
	int m_TesselationEvaluationShaderID;
	

};