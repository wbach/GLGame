#ifndef CAMERA_H
#define CAMERA_H
//#include "Terrain.h"
#include <SDL2/SDL.h>
#include "glm/glm.hpp"
#include "../UTIL.h"


class CCamera{
protected:
	float distanceFromPlayer;
    float angleAroundPlayer;

	glm::vec3 position;
    float pitch ;
    float yaw ;
    float roll ;

	glm::mat4 viewMatrix ;

public:
	CCamera();
	void updateViewMatrix();
	glm::mat4 getViewMatrix();
	void setPosition(glm::vec3 position);

	//virtual void move(CTerrain *terrain = NULL);
	void invertPitch();
	glm::vec3 getPosition();
	glm::vec3 getRotation() { return glm::vec3(pitch, yaw, roll); }
	float getPitch();
	float getDistance();
	void setDistance(float dist);
	void setPitch(float pitch);

	float getYaw();

	void setYaw(float yaw);
	float getAngleAround();

	void setAngleAround(float a);
	float getRoll();
	float getAngleAroung();

	void setRoll(float roll);

	virtual void move(SDL_Window *win);
	virtual void calculateZoom(float zoomLvL) {}
	virtual void attachToObject(glm::vec3 &positionEntity, glm::vec3 &rotationEntity) = 0;

};


#endif
