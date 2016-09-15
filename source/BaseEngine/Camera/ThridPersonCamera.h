#pragma once
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>
class CThirdPersonCamera : public CCamera
{
	float mousevel;
	glm::vec3 &lookAtPosition;
	glm::vec3 &lookAtRotation;
	bool captureMouse ;
	int windowWidth, windowHeight ;

public:
	bool showCursor ;
	glm::vec3 offset;
	void setCaptureMouse(bool capture);
	CThirdPersonCamera(glm::vec3 &lookAtPositiom, glm::vec3 &lookAtRotation);
	void lockCamera();
	void attachToObject(glm::vec3 &positionEntity, glm::vec3 &rotationEntity) override;
	void calculateCameraPosition(float horizontalDistance, float verticalDistance);

	float calculateHorizontalDistance();
	float calculateVerticalDistance();
	
	glm::vec2 calcualteMouseMove(SDL_Window *win);
	void calculatePitch(glm::vec2 dmove );
	void calculateAngleAroundPlayer(glm::vec2 dmove );

	void move(SDL_Window *win) override;
	void calculateZoom(float zoomLvL) override;
};
