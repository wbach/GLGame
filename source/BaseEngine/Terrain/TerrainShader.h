#ifndef TERRAIN_SHADER_H
#define TERRAIN_SHADER_H
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "../Lights/Light.h"
#include "../Shaders/ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"


class TerrainShader : public CShaderProgram{
	int location_isElementOfTerrain;
	int location_useFakeLighting;
	int location_skyColour;
	int numberOfLights ;
	int location_transformationMatrix ;
    int location_projectionMatrix ;
    int location_viewMatrix ;
	int location_numberOfLights;
	int location_useNormalMap;
	int location_backgroundTexture;
	int location_rTexture;
	int location_bTexture;
	int location_gTexture;
	int location_blendMap;
	int location_backgroundTextureNormal;
	int location_rTextureNormal;
	int location_bTextureNormal;
	int location_gTextureNormal;
	int location_numberOfRows;
	int location_offset;
	int location_toShadowMapSpace ;
	int location_shadowMap;
	int location_isShadows;
	int location_viewDistance ;
	int location_plane; // clipdistance
	vector<int> location_lightColour ;
	vector<int> location_lightPosition ;
	vector<int> location_attenuation ;
public:
	TerrainShader(){}
	void init(int numberOfLights);
	void getAllUniformLocations();
	void loadViewDistance(float distance);
	void bindAttributes();
	void loadTransformMatrix(glm::mat4 matrix);
    void loadProjectionMatrix(glm::mat4 matrix);
	void loadViewMatrix(glm::mat4 matrix);
	void loadLight(CLight light, int i);
	void loadLightNumber(float i) ;
	void loadUseNormalMap(float use) ;
	void connectTextureUnits();
	void loadSkyColour(float r, float g, float b);
	void loadUseFakeLight(float use);
	void loadIsElementOfTerrain(float is);
	void loadNumberOfRows(float numberOfRows);
	void loadOffset(glm::vec2 offset);
	void loadToShadowSpaceMatrix(glm::mat4 matrix);
	void loadClipPlaneVector(glm::vec4 plane);
	void loadIsShadows(float is);
	~TerrainShader();

   // void loadViewMatrix(Camera camera);
};

#endif
