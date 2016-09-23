#pragma once
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>
static glm::vec3 zero(0);

class CFirstPersonCamera : public CCamera
{	
public:
	CFirstPersonCamera();
	CFirstPersonCamera(glm::vec3& position_entity, glm::vec3& rotation_entity);

	void Move() override;
	void AttachToObject(glm::vec3& position_entity, glm::vec3& rotation_entity) override;	

private:
	void MoveCamera(float dist, float dir);
	void MoveCameraUp(float dist, float dir);	
	void LockCamera();
	glm::vec2 CalcualteMouseMove();

	float m_Mousevel;
	float m_Movevel;

	glm::vec3& m_LookPosition;
	glm::vec3& m_LookRotation;

	bool m_IsFreeCamera = true;
};

