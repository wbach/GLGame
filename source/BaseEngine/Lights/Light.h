#pragma once
#include "glm/glm.hpp"
namespace LightType
{
	enum {
		DIRECTIONAL_LIGHT = 0,
		POINT_LIGHT,
		SPOT_LIGHT
	};
}

class CLight{
	int m_Type ;
	glm::vec3 m_Position ;
    glm::vec3 m_Colour ;
    glm::vec3 m_Attenuation;
	float m_CutOff;
public:
	CLight();
	CLight(glm::vec3 colour);
	CLight(glm::vec3 position,glm::vec3 colour);
	CLight(glm::vec3 position,glm::vec3 colour,glm::vec3 attenuation);
	CLight(glm::vec3 position,glm::vec3 colour,glm::vec3 attenuation,float cutOff);
	const int& GetType();
	const glm::vec3& GetPosition();
	const glm::vec3& GetColour();
	const glm::vec3& GetIColour(){return m_Colour;}
	const glm::vec3& GetAttenuation();
	const float& GetCutoff();
};