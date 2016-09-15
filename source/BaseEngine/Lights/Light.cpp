#include "Light.h"



CLight::CLight(glm::vec3 position,glm::vec3 colour,glm::vec3 attenuation,float cutOff)
{
	this->type = SPOT_LIGHT;
	this->position = position;
	this->colour = colour;	
	this->attenuation = attenuation;
	this->cutOff = cutOff;
}

CLight::CLight(glm::vec3 position,glm::vec3 colour,glm::vec3 attenuation)
{
	this->type = POINT_LIGHT;
	this->position = position;
	this->colour = colour;	
	this->attenuation = attenuation;
}

CLight::CLight(glm::vec3 position,glm::vec3 colour)
{
	this->type = DIRECTIONAL_LIGHT;
	this->position = position;
	this->colour = colour;
	this->attenuation = glm::vec3(1,0,0);
}
CLight::CLight(glm::vec3 colour)
{
	this->type = DIRECTIONAL_LIGHT;
	this->position = glm::vec3(100000,150000,100000);
	this->colour = colour;
	this->attenuation = glm::vec3(1,0,0);
}
CLight::CLight()
{
	this->type = DIRECTIONAL_LIGHT;
	this->position = glm::vec3(100000,150000,100000);
	this->colour = glm::vec3(0.8);
	this->attenuation = glm::vec3(1,0,0);
}

int CLight::getType()
{
	return type;
}

glm::vec3 CLight::getPosition()
{
	return position;
}

glm::vec3 CLight::getColour()
{
	return colour;
}

glm::vec3 CLight::getAttenuation()
{
	return attenuation;
}

float CLight::getCutoff()
{
	return cutOff;
}
