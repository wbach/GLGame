#pragma once
#include "ShaderProgram.h"
#include <vector>
#include "../Lights/Light.h"
static const int MAX_LIGHTS = 10;
class CLightPassShader : public CShaderProgram
{
public:
	void GetAllUniformLocations() override;
	void BindAttributes() override;

	void Init();
	void LoadTransformMatrix(const glm::mat4&) const;
	void LoadProjectionMatrix(const glm::mat4&) const;
	void LoadViewMatrix(const glm::mat4&) const;
	void LoadLightNumber(const int& i) const;
	void LoadLight(const CLight& light, const int& i) const;
	void LoadScreenSize(glm::vec2 screen_size);
	void ConnectTextureUnits() const;
private:
	int m_NumberOfLights;
	int location_ScreenSize;

	int location_TransformationMatrix;
	int location_ProjectionMatrix;
	int location_ViewMatrix;

	int location_PositionMap;
	int location_ColorMap;
	int location_NormalMap;

	int location_NumberOfLights;
	std::vector<int> location_LightType;
	std::vector<int> location_LightPosition;
	std::vector<int> location_LightColour;
	std::vector<int> location_LightAttenuation;
	std::vector<int> location_LightCutOff;
};
