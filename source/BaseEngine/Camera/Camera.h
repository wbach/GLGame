#pragma once
#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include "../Utils/Utils.h"

class CCamera{
public:
	virtual void Move(SDL_Window* win);
	virtual void CalculateZoom(float zoomLvL) {}
	virtual void AttachToObject(glm::vec3& positionEntity, glm::vec3& rotationEntity) = 0;

	CCamera();
	void UpdateViewMatrix();
	void InvertPitch();	

	float GetDistance();
	float GetAngleAround();
	void SetAngleAround(float a);
	void SetDistance(float dist);
	void SetPitch(float pitch);

	float GetPitch();
	float GetYaw();
	float GetRoll();
	
	void SetRoll(float roll);
	void SetYaw(float yaw);
	
	glm::vec3 GetPosition();
	glm::vec3 GetRotation() { return glm::vec3(m_Pitch, m_Yaw, m_Roll); }
	glm::mat4 GetViewMatrix();

	void SetPosition(glm::vec3 position);

protected:
	float m_DistanceFromPlayer;
	float m_AngleAroundPlayer;

	float m_Pitch;
	float m_Yaw;
	float m_Roll;

	glm::vec3 m_Position;
	glm::mat4 m_ViewMatrix;
};
