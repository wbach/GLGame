#include "TerrainGeometryPassShader.h"



void CTerrainGeometryPassShader::Init()
{
	InitShaderProgram("Data/Shaders/TerrainGeometryPassShader.vert", "Data/Shaders/TerrainGeometryPassShader.frag");
	Start();
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}
void CTerrainGeometryPassShader::ConnectTextureUnits() const
{
	LoadValue(location_backgroundTexture, 0);
	LoadValue(location_rTexture, 1);
	LoadValue(location_gTexture, 2);
	LoadValue(location_bTexture, 3);
	LoadValue(location_blendMap, 4);
	LoadValue(location_backgroundTextureNormal, 5);
	LoadValue(location_rTextureNormal, 6);
	LoadValue(location_gTextureNormal, 7);
	LoadValue(location_bTextureNormal, 8);
	LoadValue(location_shadowMap, 9);
}
void CTerrainGeometryPassShader::GetAllUniformLocations()
{
	// Get location of matrixes
	location_transformationMatrix = GetUniformLocation("TransformationMatrix");
	location_projectionMatrix = GetUniformLocation("ProjectionMatrix");
	location_viewMatrix = GetUniformLocation("ViewMatrix");

	//Get location of samplers 2d
	location_blendMap = GetUniformLocation("BlendMap");

	location_backgroundTexture = GetUniformLocation("BackgroundTexture");
	location_rTexture = GetUniformLocation("rTexture");
	location_bTexture = GetUniformLocation("bTexture");
	location_gTexture = GetUniformLocation("gTexture");
	
	location_backgroundTextureNormal = GetUniformLocation("BackgroundTextureNormal");
	location_rTextureNormal = GetUniformLocation("rTextureNormal");
	location_bTextureNormal = GetUniformLocation("bTextureNormal");
	location_gTextureNormal = GetUniformLocation("gTextureNormal");

	location_shadowMap = GetUniformLocation("ShadowMap");
}

void CTerrainGeometryPassShader::BindAttributes()
{
	BindAttribute(0, "Position");
	BindAttribute(1, "TexCoord");
	BindAttribute(2, "Normal");
	BindAttribute(3, "Tangent");
}
void CTerrainGeometryPassShader::LoadTransformMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_transformationMatrix, matrix);
}

void CTerrainGeometryPassShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_projectionMatrix, matrix);
}

void CTerrainGeometryPassShader::LoadViewMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_viewMatrix, matrix);
}
