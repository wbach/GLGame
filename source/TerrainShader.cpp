#include "TerrainShader.h"

void TerrainShader::init(int numberOfLights){
	//initShaderProgram("Data/Shaders/terrainShader.vs" ,"Data/Shaders/terrainShader.fs", "Data/Shaders/terrainShader.ts", "Data/Shaders/terrainShader.tes") ;
	initShaderProgram("Data/Shaders/terrainShader.vs", "Data/Shaders/terrainShader.fs");
	this->numberOfLights = numberOfLights;
	start() ;
	getAllUniformLocations();
	connectTextureUnits();
	stop();
	//
}
void TerrainShader::getAllUniformLocations(){
	location_isElementOfTerrain = getUniformLocation("isElementOfTerrain");
	location_useFakeLighting = getUniformLocation("useFakeLighting");
	location_skyColour = getUniformLocation("skyColour");
	location_transformationMatrix = getUniformLocation("transformationMatrix");
    location_projectionMatrix = getUniformLocation("projectionMatrix");
    location_viewMatrix = getUniformLocation("viewMatrix");
	location_numberOfLights = getUniformLocation("numberOfLights");
	location_useNormalMap = getUniformLocation("useNormalMap");
	location_backgroundTexture = getUniformLocation("backgroundTexture");
	location_rTexture = getUniformLocation("rTexture");
	location_bTexture = getUniformLocation("bTexture");
	location_gTexture = getUniformLocation("gTexture");
	location_blendMap = getUniformLocation("blendMap");
	location_backgroundTextureNormal = getUniformLocation("backgroundTextureNormal");
	location_rTextureNormal = getUniformLocation("rTextureNormal");
	location_bTextureNormal = getUniformLocation("bTextureNormal");
	location_gTextureNormal = getUniformLocation("gTextureNormal");
	location_numberOfRows = getUniformLocation("numberOfRows");
	location_offset = getUniformLocation("offset");
	location_toShadowMapSpace = getUniformLocation("toShadowMapSpace");
	location_shadowMap = getUniformLocation("shadowMap");
	location_plane = getUniformLocation("plane");
	location_isShadows = getUniformLocation("isShadows");
	location_viewDistance = getUniformLocation("viewDistance");
	for(int i =0 ; i < numberOfLights; i++){
		char tmpVariableName[50] ; ; memset(tmpVariableName,0,50) ;
		sprintf(tmpVariableName,"lightColour[%i]",i);
        location_lightColour.push_back(getUniformLocation(tmpVariableName));
		sprintf(tmpVariableName,"lightPosition[%i]",i);
        location_lightPosition.push_back(getUniformLocation(tmpVariableName));
		sprintf(tmpVariableName,"attenuation[%i]",i);
        location_attenuation.push_back(getUniformLocation(tmpVariableName));

       }

}

void TerrainShader::loadViewDistance(float distance)
{
	loadValue(location_viewDistance,distance);
}

void TerrainShader::loadUseFakeLight(float use){
	loadValue(location_useFakeLighting,use);
}

void TerrainShader::loadIsElementOfTerrain(float is)
{
	loadValue(location_isElementOfTerrain,is);
}
void TerrainShader::loadNumberOfRows(float numberOfRows){
	loadValue(location_numberOfRows,numberOfRows);
}
void TerrainShader::loadOffset(glm::vec2 offset){
	loadValue(location_offset,offset);
}

void TerrainShader::loadToShadowSpaceMatrix(glm::mat4 matrix)
{
	loadValue(location_toShadowMapSpace, matrix);
}

void TerrainShader::loadClipPlaneVector(glm::vec4 plane)
{
	loadValue(location_plane, plane);
}

void TerrainShader::loadIsShadows(float is)
{
	loadValue(location_isShadows,is);
}

void TerrainShader::loadSkyColour(float r, float g, float b)
{
	loadValue(location_skyColour,glm::vec3(r,g,b));
}
void TerrainShader::connectTextureUnits(){
		loadValue(location_backgroundTexture, 0);
		loadValue(location_rTexture, 1);
		loadValue(location_gTexture, 2);
		loadValue(location_bTexture, 3);
		loadValue(location_blendMap, 4);
		loadValue(location_backgroundTextureNormal,5);
		loadValue(location_rTextureNormal,6);
		loadValue(location_gTextureNormal,7);
		loadValue(location_bTextureNormal,8);
		loadValue(location_shadowMap, 9);
	}
void TerrainShader::loadUseNormalMap(float use) {
	loadValue(location_useNormalMap,use) ;
}
void TerrainShader::loadLightNumber(float i) {
	loadValue(location_numberOfLights,i);
}
void TerrainShader::loadLight(CLight light, int i) {
	loadValue(location_attenuation[i],light.getAttenuation()) ;
	loadValue(location_lightPosition[i],light.getPosition()) ;
	loadValue(location_lightColour[i],light.getColour()) ;
}
void TerrainShader::bindAttributes(){
	bindAttribute(0, "position");
}
void TerrainShader::loadTransformMatrix(glm::mat4 matrix){
    loadValue(location_transformationMatrix, matrix);
}
void TerrainShader::loadProjectionMatrix(glm::mat4 matrix){
    loadValue(location_projectionMatrix, matrix);
}
void TerrainShader::loadViewMatrix(glm::mat4 matrix){
	loadValue(location_viewMatrix, matrix);
}
TerrainShader::~TerrainShader(){
}
