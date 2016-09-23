#pragma once
#include "OpenGLRenderer.h"
#include "SDLOpenGL.h"
#include <iostream>
#include <thread>
#include "glm/glm.hpp"

using namespace std;
namespace API
{
	enum 
	{
		SDL2 = 0,
		GLFW3,
		GLUT
	};
}
namespace Renderer
{
	enum
	{
		OPENGL = 0,
		DIRECTX
	};
}
class CDisplayManager 
{
public:
	int Initialize(int api, int renderer, int w, int h);
	void Update();
	void Uninitialize();

	void SetFullScreen();
	void CalculateFPS();
	const int GetFps();
	const int& GetFPSCap() { return m_FPS_CAP; }

	const float GetCurrentTime();
	const float GetDeltaTime() const { return (m_Delta /1000.0f); } 

	const glm::vec2& GetWindowSize();

	void ShowCoursor(bool show);
	bool CheckActiveWindow();

	void SetInput(std::shared_ptr<CInput>&);
private:	
	std::shared_ptr<CApi> m_Api;
	std::shared_ptr<CRenderer> m_Renderer;

	float m_LastFrameTime;
	float m_Delta;
	Uint32 m_CurrentTime, m_PreviousTime;
	float m_FrameCount, m_Fps;

	int m_FPS_CAP;
	bool m_IsFullScreen;
	glm::vec2 m_WindowsSize;
};