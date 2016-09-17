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
	void LoadIsTextured(float isTex);
	void LoadViewDistance(float distance);
	void GetAllUniformLocations();
	void BindAttributes();

	void LoadTransformMatrix(const glm::mat4& matrix);
    void LoadProjectionMatrix(const glm::mat4& matrix);
	void LoadViewMatrix(const glm::mat4& viewMatrix);
	void LoadToShadowSpaceMatrix(const glm::mat4& matrix);

	void LoadLight(CLight& light, int i);
	void LoadLightNumber(float i) ;
	void LoadUseNormalMap(float use) ;
	void ConnectTextureUnits();
	void LoadSkyColour(float r, float g, float b);
	void LoadUseFakeLight(float use);
	void LoadMaterial(glm::vec3 diffuse, glm::vec3 specular, float shineDamper, float reflectivity);
	void LoadNumberOfRows(float numberOfRows);
	void LoadOffset(glm::vec2 offset);	
	void LoadClipPlaneVector(glm::vec4& plane);
	void LoadIsShadows(float is);
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
