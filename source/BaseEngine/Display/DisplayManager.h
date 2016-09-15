#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H
#pragma once
#include <string>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <thread>
#include "glm/glm.hpp"

using namespace std;

class CDisplayManager {
public:
	SDL_GLContext glContext,glLoadingContext;
	SDL_atomic_t stuffIsReady;

	long lastFrameTime;
	float delta;
	Uint32 currentTime, previousTime;
	float frameCount,fps;

	SDL_Window *window;
	int FPS_CAP;
	bool fullScreen;
	glm::vec2 windowsSize;
    void calculateFPS();
	int getFps();
	int getFPSCap() { return FPS_CAP; }
	int initialize(int w, int h);
	void update();
	void uninitialize();
	void setFullScreen();
	float getCurrentTime();
	float getDeltaTime() { return static_cast<float>(delta/1000); }
	glm::vec2 getWindowSize();
	SDL_Window * getWindow() { return window; }
	SDL_GLContext getLoadingContext() { return glLoadingContext; }
};
#endif // !DISPLAY_MANAGER_H


#pragma once
