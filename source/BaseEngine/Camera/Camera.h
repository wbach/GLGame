#pragma once
#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include "../Utils/Utils.h"

class CCamera{
public:
	virtual void Move(SDL_Window* win);
	virtual void CalculateZoom(float zoom_lvl) {}
	virtual void AttachToObject(glm::vec3& position_entity, glm::vec3& rotation_entity) = 0;

	CCamera();
	void UpdateViewMatrix();
	void InvertPitch();	

	const float& GetDistance() const;
	const float& GetAngleAround() const;
	void SetAngleAround(float a);
	void SetDistance(float dist);
	void SetPitch(float pitch);

	const float& GetPitch() const;
	const float& GetYaw() const;
	const float& GetRoll() const;
	
	void SetRoll(float roll);
	void SetYaw(float yaw);
	
	const glm::vec3& GetPosition() const;
	const glm::vec3& GetRotation() const { return glm::vec3(m_Pitch, m_Yaw, m_Roll); }
	const glm::mat4& GetViewMatrix() const;

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
