#include "Entity.h"

void CEntity::setPosition(glm::vec3 position, int i)
{
	if (i < 0 || i > transforms.size()) return;
		transforms[i].position = position;
	calculateEntityTransformMatrix(i);
}

void CEntity::setRotation(glm::vec3 rotation, int i)
{
	if (i < 0 || i > transforms.size()) return;
	transforms[i].rotation = rotation;
	calculateEntityTransformMatrix(i);
}

void CEntity::setScale(glm::vec3 scale, int i)
{
	if (i < 0 || i > transforms.size()) return;
	transforms[i].scale = scale;
	calculateEntityTransformMatrix(i);
}

void CEntity::setTransform(Transform transform, int i)
{
	if (i < 0 || i > transforms.size()) return;
	transforms[i] = transform;
	calculateEntityTransformMatrix(i);
}

glm::vec2 CEntity::getPositionXZ(int i)
{
	if (i < 0 || i > transforms.size()) return glm::vec2(0);
	return glm::vec2(transforms[i].position.x, transforms[i].position.z);
}

glm::vec3 CEntity::getPosition(int i)
{
	if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return transforms[i].position;
}

glm::vec3 CEntity::getRotation(int i)
{
	if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return transforms[i].rotation;
}

glm::vec3 CEntity::getScale(int i)
{
	if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return transforms[i].scale;
}
glm::vec3 & CEntity::getReferencedPosition(int i)
{
	//if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return transforms[i].position;
}

glm::vec3 & CEntity::getReferencedRotation(int i)
{
	//if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return transforms[i].rotation;
}

glm::vec3 & CEntity::getReferencedScale(int i)
{
	//if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return transforms[i].scale;
}
Transform CEntity::getTransform(int i)
{
	if (i < 0 || i > transformMatrixes.size()) return Transform();
	return transforms[i];
}

glm::mat4 CEntity::getTransformMatrix(int i) {
	if (i < 0 || i > transformMatrixes.size()) return glm::mat4(1);
	return transformMatrixes[i];
}


CEntity::CEntity() {
	addTransform(Transform());
}

CEntity::CEntity(glm::vec3 pos) {
	addTransform(Transform(pos));
}

CEntity::CEntity(glm::vec3 pos, glm::vec3 rot) {
	addTransform(Transform(pos, rot));
}

CEntity::CEntity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
	addTransform(Transform(pos, rot, scale));
}

void CEntity::addTransform(Transform transform) {
	transforms.push_back(transform);
	transformMatrixes.push_back(createTransformationMatrix(transform.position, transform.rotation, transform.scale));
}

void CEntity::incrasePosition(float dx, float dy, float dz, int index)
{
	this->transforms[index].position.x += dx;
	this->transforms[index].position.y += dy;
	this->transforms[index].position.z += dz;
	calculateEntityTransformMatrix(index);
}

void CEntity::addSubbEntity(shared_ptr<CEntity> e) {
	entities.push_back(e);
}

void CEntity::increaseRotation(float dx, float dy, float dz, int index)
{
	this->transforms[index].rotation.x += dx;
	this->transforms[index].rotation.y += dy;
	this->transforms[index].rotation.z += dz;
	calculateEntityTransformMatrix(index);
}

void CEntity::calculateEntityTransformMatrix(int x)
{
	if (x < 0 || x > transforms.size()) {
		int i = 0;
		for (Transform transform : transforms)
			transformMatrixes[i++] = createTransformationMatrix(transform.position, transform.rotation, transform.scale);
	}
	else
	{
		transformMatrixes[x] = createTransformationMatrix(transforms[x].position, transforms[x].rotation, transforms[x].scale);
	}
}
