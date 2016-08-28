#ifndef FIRST_PERSON_CAMERA_H
#define FIRST_PERSON_CAMERA_H
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>
static glm::vec3 zero(0);
class CFirstPersonCamera : public CCamera
{

	float mousevel ;
	float movevel ;

	glm::vec3 &lookPosition;
	glm::vec3 &lookRotation;

	bool freeCamera  = true;
public:
//	CFirstPersonCamera() {}
	CFirstPersonCamera();
	CFirstPersonCamera(glm::vec3 &positionEntity, glm::vec3 &rotationEntity);
	void lockCamera();
	void move(SDL_Window *win);
	void attachToObject(glm::vec3 &positionEntity, glm::vec3 &rotationEntity) {
		lookPosition = positionEntity;
		lookRotation = rotationEntity;
		freeCamera = false;
	}
	glm::vec2 calcualteMouseMove(SDL_Window *win);
	void moveCamera(float dist,float dir);
	void moveCameraUp(float dist,float dir);
};
#endif
