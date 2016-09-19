#include "EntityShader.h"

void EntityShader::Init(int number_of_lights)
{
	InitShaderProgram("Data/Shaders/enitytyShader.vs" ,"Data/Shaders/enitytyShader.fs" ) ;
	this->m_NumberOfLights = number_of_lights;
	Start() ;
	GetAllUniformLocations();
	ConnectTextureUnits();
	Stop();
}
void EntityShader::GetAllUniformLocations()
{
	location_isTextured = GetUniformLocation("isTextured");
	location_skyColour = GetUniformLocation("skyColour");
	location_transformationMatrix = GetUniformLocation("transformationMatrix");
    location_projectionMatrix = GetUniformLocation("projectionMatrix");
    location_viewMatrix = GetUniformLocation("viewMatrix");
	location_numberOfLights = GetUniformLocation("numberOfLights");
	location_useNormalMap = GetUniformLocation("useNormalMap");
	location_modelTexture = GetUniformLocation("modelTexture");
	location_normalMap = GetUniformLocation("normalMap");
	location_useFakeLighting = GetUniformLocation("useFakeLighting");
	location_modelDiffuse = GetUniformLocation("modelDiffuse");
	location_modelSpecular = GetUniformLocation("modelSpecular");
	location_shineDamper = GetUniformLocation("shineDamper");
	location_reflectivity = GetUniformLocation("reflectivity");
	location_numberOfRows = GetUniformLocation("numberOfRows");
	location_offset = GetUniformLocation("offset");
	location_viewDistance = GetUniformLocation("viewDistance");
	location_toShadowMapSpace = GetUniformLocation("toShadowMapSpace");
	location_shadowMap = GetUniformLocation("shadowMap");
	location_plane = GetUniformLocation("plane");
	location_isShadows = GetUniformLocation("isShadows");
	for(int i =0 ; i < m_NumberOfLights; i++)
	{
		char tmpVariableName[50] ; memset(tmpVariableName,0,50) ;//ZeroMemory(tmpVariableName,50) ;
		sprintf(tmpVariableName,"lightColour[%i]",i);
		location_lightColour.push_back(GetUniformLocation(tmpVariableName));
		sprintf(tmpVariableName,"lightPosition[%i]",i);
		location_lightPosition.push_back(GetUniformLocation(tmpVariableName));
		sprintf(tmpVariableName,"attenuation[%i]",i);
		location_attenuation.push_back(GetUniformLocation(tmpVariableName));
	}
}
void EntityShader::LoadIsTextured(float isTex)
{
	LoadValue(location_isTextured, isTex);
}
void EntityShader::LoadIsShadows(float is)
{
	LoadValue(location_isShadows, is);
}
void EntityShader::LoadViewDistance(float distance)
{
	LoadValue(location_viewDistance, distance);
}

void EntityShader::LoadNumberOfRows(float numberOfRows)
{
	LoadValue(location_numberOfRows, numberOfRows);
}
void EntityShader::LoadOffset(glm::vec2 offset){
	LoadValue(location_offset, offset);
}
void EntityShader::LoadMaterial(glm::vec3 diffuse, glm::vec3 specular, float shine_damper, float reflectivity)
{
	LoadValue(location_modelDiffuse, diffuse);
	LoadValue(location_modelSpecular, specular);
	LoadValue(location_shineDamper, shine_damper);
	LoadValue(location_reflectivity, reflectivity);
}
void EntityShader::LoadUseFakeLight(float use)
{
	LoadValue(location_useFakeLighting, use);
}
void EntityShader::LoadSkyColour(float r, float g, float b)
{
	LoadValue(location_skyColour,glm::vec3(r , g, b));
}
void EntityShader::LoadToShadowSpaceMatrix(const glm::mat4& matrix)
{
	LoadValue(location_toShadowMapSpace, matrix);
}

void EntityShader::LoadClipPlaneVector(glm::vec4& plane)
{
	LoadValue(location_plane, plane);
}

void EntityShader::ConnectTextureUnits()
{
	LoadValue(location_modelTexture, 0);
	LoadValue(location_normalMap, 1);
	LoadValue(location_shadowMap, 2);
}
void EntityShader::LoadUseNormalMap(float use)
{
	LoadValue(location_useNormalMap, use) ;
}
void EntityShader::LoadLightNumber(float i)
{
	LoadValue(location_numberOfLights, i);
}
void EntityShader::LoadLight(const CLight& light, int i)
{
	LoadValue(location_attenuation[i], light.GetAttenuation()) ;
	LoadValue(location_lightPosition[i], light.GetPosition()) ;
	LoadValue(location_lightColour[i], light.GetColour()) ;
}
void EntityShader::BindAttributes()
{
	BindAttribute(0, "position");
	BindAttribute(1, "textureCoords");
	BindAttribute(2, "normal");
	BindAttribute(3, "tangent");
}
void EntityShader::LoadTransformMatrix(const glm::mat4& matrix)
{
    LoadValue(location_transformationMatrix, matrix);
}
void EntityShader::LoadProjectionMatrix(const glm::mat4& matrix)
{
    LoadValue(location_projectionMatrix, matrix);
}
void EntityShader::LoadViewMatrix(const glm::mat4& view_matrix)
{
	LoadValue(location_viewMatrix, view_matrix);
}
EntityShader::~EntityShader(){
}
