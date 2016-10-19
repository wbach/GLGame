#pragma once
#include "ShaderProgram.h"

class CTerrainGeometryPassShader : public CShaderProgram
{
public:
	void GetAllUniformLocations() override;
	void BindAttributes() override;

	void Init();
	void LoadTransformMatrix(const glm::mat4&) const;
	void LoadProjectionMatrix(const glm::mat4&) const;
	void LoadViewMatrix(const glm::mat4&) const;
	void LoadToShadowSpaceMatrix(const glm::mat4& matrix) const;
	void LoadUseShadows(const float& is) const;
private:
	void ConnectTextureUnits() const;
	
	//Samplers2d location
	int location_blendMap;
	int location_backgroundTexture;
	int location_rTexture;
	int location_bTexture;
	int location_gTexture;
	
	int location_backgroundTextureNormal;
	int location_rTextureNormal;
	int location_bTextureNormal;
	int location_gTextureNormal;

	//shadows
	int location_UseShadowMap;
	int location_shadowMap;
	int location_ToShadowMapSpace;

	int location_RockTexture;
	int location_RockTextureNormal;

	// Matrixes location
	int location_transformationMatrix;
	int location_projectionMatrix;
	int location_viewMatrix;
};
