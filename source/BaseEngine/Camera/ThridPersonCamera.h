#pragma once
#include "Camera.h"
#include <stdio.h>
#include <stdlib.h>
class CThirdPersonCamera : public CCamera
{
public:
	CThirdPersonCamera(glm::vec3& lookAtPositiom, glm::vec3& lookAtRotation);

	void Move(SDL_Window* win) override;
	void AttachToObject(glm::vec3& positionEntity, glm::vec3& rotationEntity) override;
	void CalculateZoom(float zoomLvL) override;

private:
	void LockCamera();
	void SetCaptureMouse(bool capture);	
	void CalculateCameraPosition(float horizontalDistance, float verticalDistance);

	float CalculateHorizontalDistance();
	float CalculateVerticalDistance();

	glm::vec2 CalcualteMouseMove(SDL_Window* win);
	void CalculatePitch(glm::vec2 dmove);
	void CalculateAngleAroundPlayer(glm::vec2 dmove);

	glm::vec3&	m_LookAtPosition;
	glm::vec3&	m_LookAtRotation;

	bool		m_IsShowCursor;
	glm::vec3	m_Offset;
	float		m_Mousevel;
	bool		m_CaptureMouse;
	int			m_WindowWidth, m_WindowHeight;
};
