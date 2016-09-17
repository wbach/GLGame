#include "FirstPersonCamera.h"

CFirstPersonCamera::CFirstPersonCamera()
: m_LookPosition(zero)
, m_LookRotation(zero)
, m_Mousevel(0.2)
, m_Movevel(1)
, m_IsFreeCamera(true)
{
	m_Pitch = 9;
	m_Yaw	= 100;
}

CFirstPersonCamera::CFirstPersonCamera(glm::vec3& position_entity, glm::vec3& rotation_entity)
: m_LookPosition(position_entity)
, m_LookRotation(rotation_entity)
, m_Mousevel(0.2)
, m_Movevel(1)
, m_IsFreeCamera(false)
{
	m_Pitch = 9;
	m_Yaw	= 100;
}

void CFirstPersonCamera::LockCamera()
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

void CFirstPersonCamera::Move(SDL_Window* win)
{
	if (!m_IsFreeCamera) 
	{
		m_Position			 = m_LookPosition;
		m_Position.y		+= 10;
		m_Yaw				 = -m_LookRotation.y +180;

		glm::vec2 d_move	 = CalcualteMouseMove(win);
		m_LookRotation.y	+= d_move.x;
		m_Pitch				-= d_move.y;
		LockCamera();
		this->UpdateViewMatrix();
		return;
	}

	if (SDL_GetWindowFlags(win) & SDL_WINDOW_INPUT_FOCUS)
	{
		glm::vec2 dmove  = CalcualteMouseMove(win);
		m_Yaw			-= dmove.x;
		m_Pitch			-= dmove.y;
		LockCamera();
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_UP])
	{
		if(m_Pitch != 90 && m_Pitch != -90)
			MoveCamera(m_Movevel, 0.0);
		MoveCameraUp(m_Movevel, 0.0);
	}else if (state[SDL_SCANCODE_DOWN])
	{
		if (m_Pitch != 90 && m_Pitch != -90)
			MoveCamera(m_Movevel, 180.0);
		MoveCameraUp(m_Movevel, 180.0);
	}
	if (state[SDL_SCANCODE_LEFT])
	{
		MoveCamera(-m_Movevel, 90.0);
	}
	else if (state[SDL_SCANCODE_RIGHT])
	{
		MoveCamera(-m_Movevel, 270);
	}
	this->UpdateViewMatrix();
}
void CFirstPersonCamera::AttachToObject(glm::vec3& position_entity, glm::vec3& rotation_entity) {
	m_LookPosition = position_entity;
	m_LookRotation = rotation_entity;
	m_IsFreeCamera = false;
}
glm::vec2 CFirstPersonCamera::CalcualteMouseMove(SDL_Window* win)
{
	int mid_x = 320;
	int mid_y = 240;
	SDL_ShowCursor(SDL_DISABLE);

	int tmp_x,tmp_y;
	SDL_GetMouseState(&tmp_x, &tmp_y);

	glm::vec2 d_move ;
	d_move.x = m_Mousevel*(mid_x - tmp_x);
	d_move.y = m_Mousevel*(mid_y - tmp_y);

	SDL_WarpMouseInWindow(win, mid_x, mid_y);
	return d_move ;
}

void CFirstPersonCamera::MoveCamera(float dist, float dir)
{
	float rad		 = (m_Yaw +dir)*M_PI/180.0;
	m_Position.x	-= sin(-rad)*dist ;
	m_Position.z	-= cos(-rad)*dist;
}

void CFirstPersonCamera::MoveCameraUp(float dist, float dir)
{
	float rad		 = (m_Pitch +dir)*M_PI/180.0;
	m_Position.y	+= sin(-rad)*dist;
}
