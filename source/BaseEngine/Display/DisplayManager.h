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
	int Initialize(int w, int h);
	void Update();
	void Uninitialize();

	void SetFullScreen();
	void CalculateFPS();
	const int GetFps();
	const int& GetFPSCap() { return m_FPS_CAP; }

	const float GetCurrentTime();
	const float GetDeltaTime() { return (m_Delta /1000.0f); }

	const glm::vec2& GetWindowSize();
	SDL_Window* GetWindow() { return m_Window; }
	const SDL_GLContext& GetLoadingContext() { return m_GlLoadingContext; }
	const SDL_GLContext& GetGlContext() { return m_GlContext; }
private:
	SDL_GLContext m_GlContext;
	SDL_GLContext m_GlLoadingContext;

	long m_LastFrameTime;
	float m_Delta;
	Uint32 m_CurrentTime, m_PreviousTime;
	float m_FrameCount, m_Fps;

	SDL_Window* m_Window;
	int m_FPS_CAP;
	bool m_IsFullScreen;
	glm::vec2 m_WindowsSize;
};
#endif // !DISPLAY_MANAGER_H


#pragma once
