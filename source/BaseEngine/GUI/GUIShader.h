#ifndef GUI_SHADER_H
#define GUI_SHADER_H
#include <vector>
#include "../Shaders/ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"

class GUIShader : public CShaderProgram
{	
public:
	GUIShader(){}
	void Init();
	void GetAllUniformLocations() override;
	void BindAttributes() override;
	void LoadTransformMatrix(const glm::mat4& matrix) const;
	~GUIShader();
private:
	int location_transformationMatrix;
};


#endif