#ifndef TRANSFORM_H
#define TRANSFORM_H
#pragma once
#include "glm/glm.hpp"
using namespace std;
struct Transform {
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	Transform() :Transform(glm::vec3(0)) {}
	Transform(glm::vec3 pos) :Transform(pos, glm::vec3(0)) {}
	Transform(glm::vec3 pos, glm::vec3 rot) :Transform(pos, rot, glm::vec3(1)) {}
	Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : position(pos), rotation(rot), scale(scale){}
};
#endif // !ENTITY_H


#pragma once
