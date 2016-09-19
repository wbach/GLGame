#ifndef TERRAIN_SHADER_H
#define TERRAIN_SHADER_H
#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include "../Lights/Light.h"
#include "../Shaders/ShaderProgram.h"
#include "glm/gtc/matrix_transform.hpp"


class TerrainShader : public CShaderProgram
{
public:
	TerrainShader(){}
	void Init(int number_of_lights);
	void GetAllUniformLocations() override;
	void BindAttributes() override;
	
	void LoadViewDistance(const float& distance) const;
	void LoadTransformMatrix(const glm::mat4& matrix) const;
    void LoadProjectionMatrix(const glm::mat4& matrix) const;
	void LoadViewMatrix(const glm::mat4& matrix) const;
	void LoadLight(const CLight& light, const int& i) const;
	void LoadLightNumber(const float& i) const;
	void LoadUseNormalMap(const float& use) const;
	void ConnectTextureUnits() const;
	void LoadSkyColour(const float& r, const float& g, const float& b) const;
	void LoadUseFakeLight(const float& use) const;
	void LoadIsElementOfTerrain(const float& is) const;
	void LoadNumberOfRows(const float& numberOfRows) const;
	void LoadOffset(const glm::vec2& offset) const;
	void LoadToShadowSpaceMatrix(const glm::mat4& matrix) const;
	void LoadClipPlaneVector(const glm::vec4& plane) const;
	void LoadIsShadows(const float& is) const;
	~TerrainShader();
private:
	int location_isElementOfTerrain;
	int location_useFakeLighting;
	int location_skyColour;	
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
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
	int location_toShadowMapSpace;
	int location_shadowMap;
	int location_isShadows;
	int location_viewDistance;
	int location_plane; // clipdistance
	vector<int> location_lightColour;
	vector<int> location_lightPosition;
	vector<int> location_attenuation;

	int m_NumberOfLights;
};

#endif
