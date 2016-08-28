#include "Camera.h"

CCamera::CCamera() {
	distanceFromPlayer = 100;
	angleAroundPlayer = 0;
	position = glm::vec3(0, 0, 0);
	pitch = 20;
	yaw = 0;
	roll = 0;
	updateViewMatrix();
}

void CCamera::setPosition(glm::vec3 position)
{
	this->position = position;
}
void CCamera::move(SDL_Window *win){
	const Uint8* state=SDL_GetKeyboardState(NULL);
	if(state[SDL_SCANCODE_UP])
	{
	position.y+=0.02f;
	}else if(state[SDL_SCANCODE_DOWN])
	{
	position.y-=0.02f;
	}
	if(state[SDL_SCANCODE_LEFT])
	{
	position.x-=0.02f;
	}
	else if(state[SDL_SCANCODE_RIGHT])
	{
	position.x+=0.02f;
	}
	updateViewMatrix();
}


void CCamera::invertPitch()
{
	this->pitch *= -1.0f;
}

glm::vec3 CCamera::getPosition()
{
	return position;
}

float CCamera::getPitch()
{
	return pitch;
}

float CCamera::getDistance()
{
	return distanceFromPlayer;
}

void CCamera::setDistance(float dist)
{
	distanceFromPlayer = dist;
}

void CCamera::setPitch(float pitch)
{
	this->pitch = pitch;
}

float CCamera::getYaw()
{
	return yaw;
}

void CCamera::setYaw(float yaw)
{
	this->yaw = yaw;
}

float CCamera::getAngleAround()
{
	return angleAroundPlayer;
}

void CCamera::setAngleAround(float a)
{
	this->angleAroundPlayer = a;
}

float CCamera::getRoll()
{
	return roll;
}

float CCamera::getAngleAroung()
{
	return roll;
}

void CCamera::setRoll(float roll)
{
	this->roll = roll;
}

void CCamera::updateViewMatrix() {
	viewMatrix = glm::mat4(1.0);
	viewMatrix *= glm::rotate(pitch, 1.0f, 0.0f, 0.0f);
	viewMatrix *= glm::rotate(yaw, 0.0f, 1.0f, 0.0f);
	viewMatrix *= glm::rotate(roll, 0.0f, 0.0f, 1.0f);
	viewMatrix *= glm::translate(-position);
}

glm::mat4 CCamera::getViewMatrix()
{
	return viewMatrix;
}

