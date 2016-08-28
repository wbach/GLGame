#ifndef ENTITY_H
#define ENTITY_H
#pragma once
#include <string>
#include "glm/glm.hpp"
#include "Transform.h"
#include "UTIL.h"
#include <vector>
#include <memory>
using namespace std;
class CEntity {
	vector<glm::mat4>transformMatrixes;	
	vector<Transform>transforms;
public:
	vector<shared_ptr<CEntity>>entities;	
	string name;
	int model_id;
	
	CEntity();
	CEntity(glm::vec3 pos);
	CEntity(glm::vec3 pos, glm::vec3 rot);
	CEntity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	
	
	
	void incrasePosition(float dx, float dy, float dz,int index=0);
	void addSubbEntity(shared_ptr<CEntity> e);
	void increaseRotation(float dx, float dy, float dz, int index = 0);
	void calculateEntityTransformMatrix(int x = 0);


	void addTransform(Transform transform);
	
	void setPosition(glm::vec3 position, int i = 0);
	void setRotation(glm::vec3 rotation, int i = 0);
	void setScale(glm::vec3 scale, int i = 0);
	void setTransform(Transform transform, int i = 0);

	glm::vec2 getPositionXZ(int i = 0);
	glm::vec3 getPosition(int i = 0);
	glm::vec3 getRotation(int i = 0);
	glm::vec3 getScale(int i = 0);
	Transform getTransform(int i = 0);
	glm::mat4 getTransformMatrix(int i = 0);
	vector<glm::mat4> &getTransformMatrixes() { return transformMatrixes; }
	glm::vec3& getReferencedPosition(int i = 0);
	glm::vec3& getReferencedRotation(int i = 0);
	glm::vec3& getReferencedScale(int i = 0);

};
#endif // !ENTITY_H


#pragma once
