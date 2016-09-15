#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "../Shaders/ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"
#include "../Lights/Light.h"
class EntityShader : public CShaderProgram{

	int location_isTextured ;
	int location_modelDiffuse ;
	int location_modelSpecular;
	int location_shineDamper;
	int location_reflectivity;
	int location_useFakeLighting;
	int location_skyColour;
	int numberOfLights ;
	int location_transformationMatrix ;
    int location_projectionMatrix ;
    int location_viewMatrix ;
	int location_numberOfLights;
	int location_useNormalMap;
	int location_modelTexture;
	int location_normalMap;
	int location_numberOfRows;
	int location_offset;
	int location_viewDistance ;
	int location_toShadowMapSpace ;
	int location_shadowMap;
	int location_isShadows;
	int location_plane; // clipdistance
	vector<int> location_lightColour ;
	vector<int> location_lightPosition ;
	vector<int> location_attenuation ;
public:
	EntityShader(){}
	void init(int numberOfLights);
	void loadIsTextured(float isTex);
	void loadViewDistance(float distance);
	void getAllUniformLocations();
	void bindAttributes();
	void loadTransformMatrix(glm::mat4 matrix);
    void loadProjectionMatrix(glm::mat4 matrix);
	void loadViewMatrix(glm::mat4 viewMatrix);
	void loadLight(CLight light, int i);
	void loadLightNumber(float i) ;
	void loadUseNormalMap(float use) ;
	void connectTextureUnits();
	void loadSkyColour(float r, float g, float b);
	void loadUseFakeLight(float use);
	void loadMaterial(glm::vec3 diffuse, glm::vec3 specular, float shineDamper, float reflectivity);
	void loadNumberOfRows(float numberOfRows);
	void loadOffset(glm::vec2 offset);
	void loadToShadowSpaceMatrix(glm::mat4 matrix);
	void loadClipPlaneVector(glm::vec4 plane);
	void loadIsShadows(float is);
	~EntityShader();

   // void loadViewMatrix(Camera camera);
};
