#include "TerrainShader.h"

void TerrainShader::Init(int number_of_lights)
{
	//initShaderProgram("Data/Shaders/terrainShader.vs" ,"Data/Shaders/terrainShader.fs", "Data/Shaders/terrainShader.ts", "Data/Shaders/terrainShader.tes") ;
	InitShaderProgram("Data/Shaders/terrainShader.vs", "Data/Shaders/terrainShader.fs");
	m_NumberOfLights = number_of_lights;
	Start() ;
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}
void TerrainShader::GetAllUniformLocations()
{
	location_isElementOfTerrain = GetUniformLocation("isElementOfTerrain");
	location_useFakeLighting = GetUniformLocation("useFakeLighting");
	location_skyColour = GetUniformLocation("skyColour");
	location_transformationMatrix = GetUniformLocation("transformationMatrix");
    location_projectionMatrix = GetUniformLocation("projectionMatrix");
    location_viewMatrix = GetUniformLocation("viewMatrix");
	location_numberOfLights = GetUniformLocation("numberOfLights");
	location_useNormalMap = GetUniformLocation("useNormalMap");
	location_backgroundTexture = GetUniformLocation("backgroundTexture");
	location_rTexture = GetUniformLocation("rTexture");
	location_bTexture = GetUniformLocation("bTexture");
	location_gTexture = GetUniformLocation("gTexture");
	location_blendMap = GetUniformLocation("blendMap");
	location_backgroundTextureNormal = GetUniformLocation("backgroundTextureNormal");
	location_rTextureNormal = GetUniformLocation("rTextureNormal");
	location_bTextureNormal = GetUniformLocation("bTextureNormal");
	location_gTextureNormal = GetUniformLocation("gTextureNormal");
	location_numberOfRows = GetUniformLocation("numberOfRows");
	location_offset = GetUniformLocation("offset");
	location_toShadowMapSpace = GetUniformLocation("toShadowMapSpace");
	location_shadowMap = GetUniformLocation("shadowMap");
	location_plane = GetUniformLocation("plane");
	location_isShadows = GetUniformLocation("isShadows");
	location_viewDistance = GetUniformLocation("viewDistance");
	for(int i =0 ; i < m_NumberOfLights; i++){
		char tmpVariableName[50] ; ; memset(tmpVariableName,0,50) ;
		sprintf(tmpVariableName,"lightColour[%i]",i);
        location_lightColour.push_back(GetUniformLocation(tmpVariableName));
		sprintf(tmpVariableName,"lightPosition[%i]",i);
        location_lightPosition.push_back(GetUniformLocation(tmpVariableName));
		sprintf(tmpVariableName,"attenuation[%i]",i);
        location_attenuation.push_back(GetUniformLocation(tmpVariableName));
       }
}

void TerrainShader::LoadViewDistance(const float& distance) const
{
	LoadValue(location_viewDistance, distance);
}

void TerrainShader::LoadUseFakeLight(const float& use)const 
{
	LoadValue(location_useFakeLighting, use);
}

void TerrainShader::LoadIsElementOfTerrain(const float& is)const
{
	LoadValue(location_isElementOfTerrain, is);
}
void TerrainShader::LoadNumberOfRows(const float& number_of_rows) const 
{
	LoadValue(location_numberOfRows, number_of_rows);
}
void TerrainShader::LoadOffset(const glm::vec2& offset) const 
{
	LoadValue(location_offset, offset);
}

void TerrainShader::LoadToShadowSpaceMatrix(const glm::mat4& matrix) const
{
	LoadValue(location_toShadowMapSpace, matrix);
}

void TerrainShader::LoadClipPlaneVector(const glm::vec4& plane)const
{
	LoadValue(location_plane, plane);
}

void TerrainShader::LoadIsShadows(const float& is) const
{
	LoadValue(location_isShadows, is);
}

void TerrainShader::LoadSkyColour(const float& r, const float& g, const float& b) const
{
	LoadValue(location_skyColour,glm::vec3(r,g,b));
}
void TerrainShader::ConnectTextureUnits() const
{
		LoadValue(location_backgroundTexture, 0);
		LoadValue(location_rTexture, 1);
		LoadValue(location_gTexture, 2);
		LoadValue(location_bTexture, 3);
		LoadValue(location_blendMap, 4);
		LoadValue(location_backgroundTextureNormal,5);
		LoadValue(location_rTextureNormal,6);
		LoadValue(location_gTextureNormal,7);
		LoadValue(location_bTextureNormal,8);
		LoadValue(location_shadowMap, 9);
	}
void TerrainShader::LoadUseNormalMap(const float& use) const
{
	LoadValue(location_useNormalMap, use) ;
}
void TerrainShader::LoadLightNumber(const float& i) const
{
	LoadValue(location_numberOfLights, i);
}
void TerrainShader::LoadLight(const CLight& light, const int& i) const
{
	LoadValue(location_attenuation[i], light.GetAttenuation()) ;
	LoadValue(location_lightPosition[i], light.GetPosition()) ;
	LoadValue(location_lightColour[i], light.GetColour()) ;
}
void TerrainShader::BindAttributes()
{
	BindAttribute(0, "position");
}
void TerrainShader::LoadTransformMatrix(const glm::mat4& matrix) const 
{
    LoadValue(location_transformationMatrix, matrix);
}
void TerrainShader::LoadProjectionMatrix(const glm::mat4& matrix) const
{
    LoadValue(location_projectionMatrix, matrix);
}
void TerrainShader::LoadViewMatrix(const glm::mat4& matrix) const 
{
	LoadValue(location_viewMatrix, matrix);
}
TerrainShader::~TerrainShader()
{
}
