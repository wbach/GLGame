#pragma once
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>
static glm::vec3 zero(0);

class CFirstPersonCamera : public CCamera
{	
public:
	CFirstPersonCamera();
	CFirstPersonCamera(glm::vec3& positionEntity, glm::vec3& rotationEntity);
	
	void Move(SDL_Window* win) override;
	void AttachToObject(glm::vec3& positionEntity, glm::vec3& rotationEntity) override;	

private:
	void MoveCamera(float dist, float dir);
	void MoveCameraUp(float dist, float dir);	
	void LockCamera();
	glm::vec2 CalcualteMouseMove(SDL_Window* win);

	float m_Mousevel;
	float m_Movevel;

	glm::vec3& m_LookPosition;
	glm::vec3& m_LookRotation;

	bool m_IsFreeCamera = true;
};

