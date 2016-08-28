#ifndef LIGHT_H
#define LIGHT_H
#include "glm/glm.hpp"
enum{
	DIRECTIONAL_LIGHT = 0 ,
	POINT_LIGHT,
	SPOT_LIGHT
};
class CLight{
	int type ;
	glm::vec3 position ;
    glm::vec3 colour ;
    glm::vec3 attenuation;
	float cutOff;
public:
	CLight();
	CLight(glm::vec3 colour);
	CLight(glm::vec3 position,glm::vec3 colour);
	CLight(glm::vec3 position,glm::vec3 colour,glm::vec3 attenuation);
	CLight(glm::vec3 position,glm::vec3 colour,glm::vec3 attenuation,float cutOff);
	int getType();
	glm::vec3 getPosition();
	glm::vec3 getColour();
	glm::vec3* getIColour(){return &colour;}
	glm::vec3 getAttenuation();
	float getCutoff();


};

#endif