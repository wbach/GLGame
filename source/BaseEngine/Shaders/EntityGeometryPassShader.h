#pragma once
#include "ShaderProgram.h"
#include "../Loader/Material.h"
class CEntityGeometryPassShader : public CShaderProgram
{
public:
	void GetAllUniformLocations() override;
	void ConnectTextureUnits();
	void BindAttributes() override;

	void Init();
	void LoadTransformMatrix(const glm::mat4&) const;
	void LoadProjectionMatrix(const glm::mat4&) const;
	void LoadViewMatrix(const glm::mat4&) const;

	void LoadUseInstancedRendering(const float& use) const;

	void LoadUseNormalMap(const float& use) const;
	void LoadMeshMaterial(const SMaterial& material) const;

	void LoadUseShadows(const float& is) const;
	void LoadToShadowSpaceMatrix(const glm::mat4& matrix) const;
private:
	int location_TransformationMatrix;
	int location_ProjectionMatrix;
	int location_ViewMatrix;

	//Textures locations
	int location_ModelTexture;
	int location_UseNormalMap;
	int location_NormalMap;

	//Shadows Variables
	int location_UseShadowMap;
	int location_ShadowMap;
	int location_ToShadowMapSpace;

	//Material Locations
	int location_MaterialAmbient;
	int location_MaterialDiffuse;
	int location_MaterialSpecular;

	int location_IsInstancedRender;
};
