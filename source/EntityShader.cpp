#include "EntityShader.h"

void EntityShader::init(int numberOfLights){
	initShaderProgram("Data/Shaders/enitytyShader.vs" ,"Data/Shaders/enitytyShader.fs" ) ;
	this->numberOfLights = numberOfLights;
	start() ;
	getAllUniformLocations();
	connectTextureUnits();
	stop();
	//
}
void EntityShader::getAllUniformLocations(){
	location_isTextured = getUniformLocation("isTextured");
	location_skyColour = getUniformLocation("skyColour");
	location_transformationMatrix = getUniformLocation("transformationMatrix");
    location_projectionMatrix = getUniformLocation("projectionMatrix");
    location_viewMatrix = getUniformLocation("viewMatrix");
	location_numberOfLights = getUniformLocation("numberOfLights");
	location_useNormalMap = getUniformLocation("useNormalMap");
	location_modelTexture = getUniformLocation("modelTexture");
	location_normalMap = getUniformLocation("normalMap");
	location_useFakeLighting = getUniformLocation("useFakeLighting");
	location_modelDiffuse = getUniformLocation("modelDiffuse");
	location_modelSpecular = getUniformLocation("modelSpecular");
	location_shineDamper = getUniformLocation("shineDamper");
	location_reflectivity = getUniformLocation("reflectivity");
	location_numberOfRows = getUniformLocation("numberOfRows");
	location_offset = getUniformLocation("offset");
	location_viewDistance = getUniformLocation("viewDistance");
	location_toShadowMapSpace = getUniformLocation("toShadowMapSpace");
	location_shadowMap = getUniformLocation("shadowMap");
	location_plane = getUniformLocation("plane");
	location_isShadows = getUniformLocation("isShadows");
	for(int i =0 ; i < numberOfLights; i++){
		char tmpVariableName[50] ; memset(tmpVariableName,0,50) ;//ZeroMemory(tmpVariableName,50) ;
		sprintf(tmpVariableName,"lightColour[%i]",i);
		location_lightColour.push_back(getUniformLocation(tmpVariableName));
		sprintf(tmpVariableName,"lightPosition[%i]",i);
		location_lightPosition.push_back(getUniformLocation(tmpVariableName));
		sprintf(tmpVariableName,"attenuation[%i]",i);
		location_attenuation.push_back(getUniformLocation(tmpVariableName));

	}

}
void EntityShader::loadIsTextured(float isTex){
	loadValue(location_isTextured,isTex);

}
void EntityShader::loadIsShadows(float is)
{
	loadValue(location_isShadows,is);
}
void EntityShader::loadViewDistance(float distance)
{
	loadValue(location_viewDistance,distance);
}

void EntityShader::loadNumberOfRows(float numberOfRows){
	loadValue(location_numberOfRows,numberOfRows);
}
void EntityShader::loadOffset(glm::vec2 offset){
	loadValue(location_offset,offset);
}
void EntityShader::loadMaterial(glm::vec3 diffuse, glm::vec3 specular, float shineDamper, float reflectivity){
	loadValue(location_modelDiffuse,diffuse);
	loadValue(location_modelSpecular,specular);
	loadValue(location_shineDamper,shineDamper);
	loadValue(location_reflectivity,reflectivity);
}
void EntityShader::loadUseFakeLight(float use){
	loadValue(location_useFakeLighting,use);
}
void EntityShader::loadSkyColour(float r, float g, float b)
{
	loadValue(location_skyColour,glm::vec3(r,g,b));
}
void EntityShader::loadToShadowSpaceMatrix(glm::mat4 matrix)
{
	loadValue(location_toShadowMapSpace, matrix);
}

void EntityShader::loadClipPlaneVector(glm::vec4 plane)
{
	loadValue(location_plane, plane);
}

void EntityShader::connectTextureUnits(){
		loadValue(location_modelTexture, 0);
		loadValue(location_normalMap, 1);
		loadValue(location_shadowMap, 2);
	}
void EntityShader::loadUseNormalMap(float use) {
	loadValue(location_useNormalMap,use) ;
}
void EntityShader::loadLightNumber(float i) {
	loadValue(location_numberOfLights,i);
}
void EntityShader::loadLight(CLight light, int i) {
	loadValue(location_attenuation[i],light.getAttenuation()) ;
	loadValue(location_lightPosition[i],light.getPosition()) ;
	loadValue(location_lightColour[i],light.getColour()) ;
}
void EntityShader::bindAttributes(){
	bindAttribute(0, "position");
	bindAttribute(1, "textureCoords");
	bindAttribute(2, "normal");
	bindAttribute(3, "tangent");
}
void EntityShader::loadTransformMatrix(glm::mat4 matrix){
    loadValue(location_transformationMatrix, matrix);
}
void EntityShader::loadProjectionMatrix(glm::mat4 matrix){
    loadValue(location_projectionMatrix, matrix);
}
void EntityShader::loadViewMatrix(glm::mat4 viewMatrix) {
	loadValue(location_viewMatrix, viewMatrix);
}
EntityShader::~EntityShader(){
}
