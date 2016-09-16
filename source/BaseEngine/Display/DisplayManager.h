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
	int initialize(int w, int h);
	void update();
	void uninitialize();

	void setFullScreen();
	void calculateFPS();
	int getFps();
	int getFPSCap() { return FPS_CAP; }

	float getCurrentTime();
	float getDeltaTime() { return static_cast<float>(delta/1000); }

	const glm::vec2& getWindowSize();
	SDL_Window * getWindow() { return window; }
	const SDL_GLContext& GetLoadingContext() { return m_GlLoadingContext; }
	const SDL_GLContext& GetGlContext() { return m_GlContext; }
private:
	SDL_GLContext m_GlContext;
	SDL_GLContext m_GlLoadingContext;

	long lastFrameTime;
	float delta;
	Uint32 currentTime, previousTime;
	float frameCount, fps;

	SDL_Window *window;
	int FPS_CAP;
	bool fullScreen;
	glm::vec2 windowsSize;
};
#endif // !DISPLAY_MANAGER_H


#pragma once
