#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "../Shaders/ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Lights/Light.h"
class EntityShader : public CShaderProgram
{
public:
	EntityShader(){}
	void Init(int numberOfLights);
	void LoadIsTextured(const float& isTex) const;
	void LoadViewDistance(const float& distance) const;
	void GetAllUniformLocations() override;
	void BindAttributes() override;

	void LoadTransformMatrix(const glm::mat4& matrix) const;
    void LoadProjectionMatrix(const glm::mat4& matrix) const;
	void LoadViewMatrix(const glm::mat4& viewMatrix) const;
	void LoadToShadowSpaceMatrix(const glm::mat4& matrix) const;

	void LoadLight(const CLight& light, const int& i) const;
	void LoadLightNumber(const float& i) const;
	void LoadUseNormalMap(const float& use) const;
	void ConnectTextureUnits();
	void LoadSkyColour(const float& r, const float& g, const float& b) const;
	void LoadUseFakeLight(const float& use) const;
	void LoadMaterial(const glm::vec3& diffuse, const glm::vec3& specular, const float& shine_damper, const float& reflectivity) const;
	void LoadNumberOfRows(const float& numberOfRows) const;
	void LoadOffset(const glm::vec2& offset) const;
	void LoadClipPlaneVector(const glm::vec4& plane) const;
	void LoadIsShadows(const float& is) const;
	~EntityShader();
private:
	int location_isTextured;
	int location_modelDiffuse;
	int location_modelSpecular;
	int location_shineDamper;
	int location_reflectivity;
	int location_useFakeLighting;
	int location_skyColour;	
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
	int location_numberOfLights;
	int location_useNormalMap;
	int location_modelTexture;
	int location_normalMap;
	int location_numberOfRows;
	int location_offset;
	int location_viewDistance;
	int location_toShadowMapSpace;
	int location_shadowMap;
	int location_isShadows;
	int location_plane; // clipdistance
	
	vector<int> location_lightColour;
	vector<int> location_lightPosition;
	vector<int> location_attenuation;

	int m_NumberOfLights;
};
