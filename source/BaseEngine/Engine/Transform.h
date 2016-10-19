#pragma once
#include "glm/glm.hpp"

struct STransform 
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	STransform() :STransform(glm::vec3(0)) {}
	STransform(glm::vec3 pos) : STransform(pos, glm::vec3(0)) {}
	STransform(glm::vec3 pos, glm::vec3 rot) : STransform(pos, rot, glm::vec3(1)) {}
	STransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : position(pos), rotation(rot), scale(scale){}
};

static STransform s_DefaultTransform;