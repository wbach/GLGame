#include "ThridPersonCamera.h"

CThirdPersonCamera::CThirdPersonCamera(glm::vec3& look_at_position, glm::vec3& look_at_rotation)
: m_Offset(0)
, m_LookAtPosition(look_at_position)
, m_LookAtRotation(look_at_rotation)
, m_Mousevel(0.2)
, m_CaptureMouse(true)
, m_IsShowCursor(false)
{
	m_DistanceFromPlayer = 20;
}

void CThirdPersonCamera::SetCaptureMouse(bool capture)
{
	m_CaptureMouse = capture;
}
void CThirdPersonCamera::LockCamera()
{
	if(m_Pitch > 90)
		m_Pitch = 90;
	if(m_Pitch < -90)
		m_Pitch = -90;
	if(m_Yaw < 0.0)
		m_Yaw += 360.0;
	if(m_Yaw > 360.0)
		m_Yaw -= 360;
}

void CThirdPersonCamera::AttachToObject(glm::vec3& position_entity, glm::vec3& rotation_entity) {
	m_LookAtPosition = position_entity;
	m_LookAtRotation = rotation_entity;
}

void CThirdPersonCamera::Move(SDL_Window* win)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (SDL_GetWindowFlags(win) & SDL_WINDOW_INPUT_FOCUS && !state[SDL_SCANCODE_LCTRL])
	{
		glm::vec2 d_move = CalcualteMouseMove(win) ;
		CalculatePitch(d_move);
		CalculateAngleAroundPlayer(d_move);
		LockCamera();
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
	if(m_IsShowCursor)
	{
		if(!m_CaptureMouse && (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)))
		{
			SDL_ShowCursor(SDL_DISABLE);
		}
		else if(!m_CaptureMouse)
		{
			SDL_ShowCursor(SDL_ENABLE);
		}
	}
	float horizontal_distance = CalculateHorizontalDistance() ;
	float vertical_distance	 = CalculateVerticalDistance() ;
	CalculateCameraPosition(horizontal_distance, vertical_distance);
	this->m_Yaw = 180 - (m_LookAtRotation.y + m_AngleAroundPlayer) ;

	this->UpdateViewMatrix();
}

void CThirdPersonCamera::CalculateCameraPosition(float horizontal_distance, float vertical_distance)
{
	float theata  = m_LookAtRotation.y  + m_AngleAroundPlayer;
	float x_offset = (float) (horizontal_distance * sin(toRadians(theata))) ;
	float z_offset = (float) (horizontal_distance * cos(toRadians(theata))) ;
	m_Position.x  = m_LookAtPosition.x - x_offset;
	m_Position.y  = m_LookAtPosition.y + vertical_distance + 10;
	m_Position.z  = m_LookAtPosition.z - z_offset;
	m_Position	 += m_Offset;
}

float CThirdPersonCamera::CalculateHorizontalDistance()
{
	return (float) (m_DistanceFromPlayer * cos(toRadians(m_Pitch))) ;
}

float CThirdPersonCamera::CalculateVerticalDistance()
{
	return (float) (m_DistanceFromPlayer * sin(toRadians(m_Pitch))) ;
}

void CThirdPersonCamera::CalculateZoom(float zoom_lvl)
{
	this->m_DistanceFromPlayer += zoom_lvl;
}

glm::vec2 CThirdPersonCamera::CalcualteMouseMove(SDL_Window *win)
{
	if(m_CaptureMouse || SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		int mid_x = 320;
		int mid_y = 240;
		SDL_ShowCursor(SDL_DISABLE);

		int tmp_x,tmp_y;
		SDL_GetMouseState(&tmp_x, &tmp_y);

		glm::vec2 dmove ;
		dmove.x = m_Mousevel * (mid_x - tmp_x);
		dmove.y = m_Mousevel * (mid_y - tmp_y);
		SDL_WarpMouseInWindow(win, mid_x, mid_y);

		return dmove ;
	}
	return glm::vec2(0) ;
}

void CThirdPersonCamera::CalculatePitch(glm::vec2 d_move)
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		m_Pitch -= d_move.y;
}

void CThirdPersonCamera::CalculateAngleAroundPlayer(glm::vec2 d_move)
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) 
	{
		float angle_change	 = d_move.x;
		m_AngleAroundPlayer -= angle_change;
	}
}
