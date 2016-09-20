#include "Camera.h"

CCamera::CCamera() 
: m_DistanceFromPlayer(99)
, m_AngleAroundPlayer(0)
, m_Position(0)
, m_Pitch(20)
, m_Yaw(0)
, m_Roll(0)
{	
	UpdateViewMatrix();
}

void CCamera::SetPosition(glm::vec3 position)
{
	m_Position = position;
}
void CCamera::Move(SDL_Window* win){
	const Uint8* state=SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_UP])
	{
		m_Position.y+=0.02f;
	}else if(state[SDL_SCANCODE_DOWN])
	{
		m_Position.y-=0.02f;
	}
	if (state[SDL_SCANCODE_LEFT])
	{
		m_Position.x-=0.02f;
	}
	else if (state[SDL_SCANCODE_RIGHT])
	{
		m_Position.x+=0.02f;
	}
	UpdateViewMatrix();
}


void CCamera::InvertPitch()
{
	m_Pitch *= -1.0f;
}

const glm::vec3& CCamera::GetPosition() const
{
	return m_Position;
}

const glm::vec3 & CCamera::GetRotation() const { return glm::vec3(m_Pitch, m_Yaw, m_Roll); }

const float& CCamera::GetPitch() const
{
	return m_Pitch;
}

const float& CCamera::GetDistance() const
{
	return m_DistanceFromPlayer;
}

void CCamera::SetDistance(float dist)
{
	m_DistanceFromPlayer = dist;
}

void CCamera::SetPitch(float pitch)
{
	m_Pitch = pitch;
}

const float& CCamera::GetYaw() const
{
	return m_Yaw;
}

void CCamera::SetYaw(float yaw)
{
	m_Yaw = yaw;
}

const float& CCamera::GetAngleAround() const
{
	return m_AngleAroundPlayer;
}

void CCamera::SetAngleAround(float a)
{
	m_AngleAroundPlayer = a;
}

const float& CCamera::GetRoll() const
{
	return m_Roll;
}
void CCamera::SetRoll(float roll)
{
	m_Roll = roll;
}

void CCamera::UpdateViewMatrix() 
{
	m_ViewMatrix  = glm::mat4(1.0);
	m_ViewMatrix *= glm::rotate(m_Pitch, 1.0f, 0.0f, 0.0f);
	m_ViewMatrix *= glm::rotate(m_Yaw, 0.0f, 1.0f, 0.0f);
	m_ViewMatrix *= glm::rotate(m_Roll, 0.0f, 0.0f, 1.0f);
	m_ViewMatrix *= glm::translate(-m_Position);
}

const glm::mat4& CCamera::GetViewMatrix() const
{
	return m_ViewMatrix;
}

