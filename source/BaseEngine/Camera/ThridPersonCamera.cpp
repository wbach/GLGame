#include "ThridPersonCamera.h"

CThirdPersonCamera::CThirdPersonCamera(glm::vec3& lookAtPosition, glm::vec3& lookAtRotation)
: m_Offset(0)
, m_LookAtPosition(lookAtPosition)
, m_LookAtRotation(lookAtRotation)
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
	if(m_Pitch>90)
		m_Pitch =90;
	if(m_Pitch<-90)
		m_Pitch =-90;
	if(m_Yaw<0.0)
		m_Yaw +=360.0;
	if(m_Yaw>360.0)
		m_Yaw -=360;
}

void CThirdPersonCamera::AttachToObject(glm::vec3& positionEntity, glm::vec3& rotationEntity) {
	m_LookAtPosition = positionEntity;
	m_LookAtRotation = rotationEntity;
}

void CThirdPersonCamera::Move(SDL_Window* win)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (SDL_GetWindowFlags(win) & SDL_WINDOW_INPUT_FOCUS && !state[SDL_SCANCODE_LCTRL])
	{
		glm::vec2 dmove = CalcualteMouseMove(win) ;
		CalculatePitch(dmove);
		CalculateAngleAroundPlayer(dmove);
		LockCamera();
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
	if(m_IsShowCursor){
		if(!m_CaptureMouse && (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))){
			SDL_ShowCursor(SDL_DISABLE);
		}else if(!m_CaptureMouse){
			SDL_ShowCursor(SDL_ENABLE);
		}
	}
	float horizontalDistance = CalculateHorizontalDistance() ;
	float verticalDistance	 = CalculateVerticalDistance() ;
	CalculateCameraPosition(horizontalDistance, verticalDistance);
	this->m_Yaw = 180 - (m_LookAtRotation.y + m_AngleAroundPlayer) ;

	this->UpdateViewMatrix();
}

void CThirdPersonCamera::CalculateCameraPosition(float horizontalDistance, float verticalDistance)
{
	float theata  = m_LookAtRotation.y  + m_AngleAroundPlayer;
	float xoffset = (float) (horizontalDistance*sin(toRadians(theata))) ;
	float zoffset = (float) (horizontalDistance*cos(toRadians(theata))) ;
	m_Position.x  = m_LookAtPosition.x - xoffset;
	m_Position.y  = m_LookAtPosition.y + verticalDistance  + 10;
	m_Position.z  = m_LookAtPosition.z - zoffset ;
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

void CThirdPersonCamera::CalculateZoom(float zoomLvL)
{
	this->m_DistanceFromPlayer += zoomLvL ;
}

glm::vec2 CThirdPersonCamera::CalcualteMouseMove(SDL_Window *win)
{
	if(m_CaptureMouse || SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		int MidX = 320;
		int MidY = 240;
		SDL_ShowCursor(SDL_DISABLE);

		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);

		glm::vec2 dmove ;
		dmove.x = m_Mousevel*(MidX-tmpx);
		dmove.y = m_Mousevel*(MidY-tmpy);
		SDL_WarpMouseInWindow(win, MidX, MidY);

		return dmove ;
	}
	return glm::vec2(0) ;
}

void CThirdPersonCamera::CalculatePitch(glm::vec2 dmove)
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		m_Pitch -= dmove.y;
}

void CThirdPersonCamera::CalculateAngleAroundPlayer(glm::vec2 dmove)
{
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) 
	{
		float angleChange	 = dmove.x;
		m_AngleAroundPlayer -= angleChange;
	}
}
