#include "ThridPersonCamera.h"



void CThirdPersonCamera::setCaptureMouse(bool capture)
{
	captureMouse = capture;
}

CThirdPersonCamera::CThirdPersonCamera(glm::vec3 &lookAtPosition, glm::vec3 &lookAtRotation): offset(0), lookAtPosition(lookAtPosition),lookAtRotation(lookAtRotation)
{
	mousevel = 0.2f;
	captureMouse = true ;
	showCursor = false ;
	distanceFromPlayer = 20;
}

void CThirdPersonCamera::lockCamera()
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

void CThirdPersonCamera::attachToObject(glm::vec3 & positionEntity, glm::vec3 & rotationEntity) {
	lookAtPosition = positionEntity;
	lookAtRotation = rotationEntity;
}

void CThirdPersonCamera::move(SDL_Window *win)
{

	const Uint8* state=SDL_GetKeyboardState(NULL);

	if (  SDL_GetWindowFlags(win) & SDL_WINDOW_INPUT_FOCUS && !state[SDL_SCANCODE_LCTRL])
	{
		glm::vec2 dmove = calcualteMouseMove(win) ;
		calculatePitch(dmove);
		calculateAngleAroundPlayer(dmove);
		lockCamera();
	}
	else
	{
		SDL_ShowCursor(SDL_ENABLE);
	}
	if(showCursor){
		if(!captureMouse && (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))){
			SDL_ShowCursor(SDL_DISABLE);
		}else if(!captureMouse){
			SDL_ShowCursor(SDL_ENABLE);
		}
	}
	float horizontalDistance = calculateHorizontalDistance() ;
	float verticalDistance = calculateVerticalDistance() ;
	calculateCameraPosition(horizontalDistance, verticalDistance);
	this->yaw = 180 - (lookAtRotation.y + angleAroundPlayer) ;

	/*if(terrain != NULL){
		float y = terrain->getHeightofTerrain(position.x, position.z) ;
		if ( position.y < y + 0.5 ){
			position.y = y + 0.5 ;
		}
	}*/

	//printf("Yaw: %f, Pitch: %f\n",this->yaw,this->pitch );
	//printf("Pos: %f, %f, %f\n",player->getPosition().x, player->getPosition().y, player->getPosition().z);
	this->updateViewMatrix();
}

void CThirdPersonCamera::calculateCameraPosition(float horizontalDistance, float verticalDistance)
{
	float theata = lookAtRotation.y  + angleAroundPlayer ;
	float xoffset = (float) (horizontalDistance*sin(toRadians(theata))) ;
	float zoffset = (float) (horizontalDistance*cos(toRadians(theata))) ;
	position.x = lookAtPosition.x - xoffset;
	position.y = lookAtPosition.y + verticalDistance  + 10;
	position.z = lookAtPosition.z - zoffset ;
	position+= offset ;
}

float CThirdPersonCamera::calculateHorizontalDistance()
{
	return (float) (distanceFromPlayer * cos(toRadians(pitch))) ;
}

float CThirdPersonCamera::calculateVerticalDistance()
{
	return (float) (distanceFromPlayer * sin(toRadians(pitch))) ;
}

void CThirdPersonCamera::calculateZoom(float zoomLvL)
{

	//float zoomLevel = Mouse.getDWheel() * 0.1f ;
	//distanceFromPlayer -= zoomLevel ;

	this->distanceFromPlayer +=zoomLvL ;
}

glm::vec2 CThirdPersonCamera::calcualteMouseMove(SDL_Window *win)
{
	if(captureMouse || SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)){
		int MidX= 320;
		int MidY= 240;
		SDL_ShowCursor(SDL_DISABLE);
		int tmpx,tmpy;
		SDL_GetMouseState(&tmpx,&tmpy);
		glm::vec2 dmove ;
		dmove.x=mousevel*(MidX-tmpx);
		dmove.y=mousevel*(MidY-tmpy);
		SDL_WarpMouseInWindow(win, MidX, MidY);
		return dmove ;
	}
	return glm::vec2(0) ;
}

void CThirdPersonCamera::calculatePitch(glm::vec2 dmove)
{
	//SDL_PumpEvents();
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		pitch -= dmove.y;
	}
}

void CThirdPersonCamera::calculateAngleAroundPlayer(glm::vec2 dmove)
{
	//SDL_PumpEvents();
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
		float angleChange = dmove.x;
		angleAroundPlayer -= angleChange;
	}
}
