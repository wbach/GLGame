#include "FirstPersonCamera.h"



CFirstPersonCamera::CFirstPersonCamera():lookPosition(zero),lookRotation(zero)
{
	mousevel = 0.2f;
	movevel = 1;
	freeCamera = true ;
	pitch = 9;
	yaw = 100;
}

CFirstPersonCamera::CFirstPersonCamera(glm::vec3 & positionEntity, glm::vec3 & rotationEntity) :lookPosition(positionEntity), lookRotation(rotationEntity)
{
	mousevel = 0.2f;
	movevel = 1;
	freeCamera = false;
	pitch = 9;
	yaw = 100;
}

void CFirstPersonCamera::lockCamera()
{

	if(pitch>90)
		pitch=90;
	if(pitch<-90)
		pitch=-90;
	if(yaw<0.0)
		yaw+=360.0;
	if(yaw>360.0)
		yaw-=360;
}

void CFirstPersonCamera::move(SDL_Window *win)
{
	if (!freeCamera) {
		position = lookPosition;
		position.y += 10;
		yaw = -lookRotation.y +180;
		printVector("Pos: ", lookPosition);

		glm::vec2 dmove = calcualteMouseMove(win);
		lookRotation.y += dmove.x;
		pitch -= dmove.y;
		lockCamera();
		this->updateViewMatrix();
		return;
	}

	if (  SDL_GetWindowFlags(win) & SDL_WINDOW_INPUT_FOCUS)
	{
		glm::vec2 dmove = calcualteMouseMove(win);
		yaw-=dmove.x;
		pitch-=dmove.y;
		lockCamera();
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}


	const Uint8* state=SDL_GetKeyboardState(NULL);

	if(state[SDL_SCANCODE_UP])
	{
		if(pitch!=90 && pitch!=-90)
			moveCamera(movevel,0.0);
		moveCameraUp(movevel,0.0);
	}else if(state[SDL_SCANCODE_DOWN])
	{
		if(pitch!=90 && pitch!=-90)
			moveCamera(movevel,180.0);
		moveCameraUp(movevel,180.0);
	}
	if(state[SDL_SCANCODE_LEFT])
	{
		moveCamera(-movevel,90.0);
	}
	else if(state[SDL_SCANCODE_RIGHT])
	{
		moveCamera(-movevel,270);
	}


	this->updateViewMatrix();
}

glm::vec2 CFirstPersonCamera::calcualteMouseMove(SDL_Window *win)
{
	int MidX=320;
	int MidY=240;
	SDL_ShowCursor(SDL_DISABLE);
	int tmpx,tmpy;
	SDL_GetMouseState(&tmpx,&tmpy);
	glm::vec2 dmove ;
	dmove.x=mousevel*(MidX-tmpx);
	dmove.y=mousevel*(MidY-tmpy);

	SDL_WarpMouseInWindow(win, MidX, MidY);
	return dmove ;
}

void CFirstPersonCamera::moveCamera(float dist,float dir)
{
	float rad=(yaw+dir)*M_PI/180.0;
	position.x-=sin(-rad)*dist ;
	position.z-=cos(-rad)*dist;
}

void CFirstPersonCamera::moveCameraUp(float dist,float dir)
{
	float rad=(pitch+dir)*M_PI/180.0;
	position.y+=sin(-rad)*dist;
}
