#pragma once
#include "OpenGLRenderer.h"
#include "SDLOpenGL.h"
#include "GLFW3_OpenGL.h"
#include <iostream>
#include <thread>
#include "glm/glm.hpp"
#include "../Input/Input.h"
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
	int PeekMessage();
	void Update();
	void Uninitialize();

	void SetFullScreen();
	void CalculateFPS();
	const int GetFps();
	const int& GetFPSCap() { return m_FPS_CAP; }

	const float GetCurrentTime();
	const double GetDeltaTime() const { return m_Delta; }

	const glm::vec2& GetWindowSize();

	void ShowCoursor(bool show);
	bool CheckActiveWindow();

	void SetInput(std::shared_ptr<CInput>&);

	bool& GetSync() { return m_Sync; }
private:	
	std::shared_ptr<CApi> m_Api;
	std::shared_ptr<CRenderer> m_Renderer;

	double m_LastFrameTime;
	double m_Delta;
	Uint32 m_CurrentTime, m_PreviousTime;
	float m_FrameCount, m_Fps;

	int m_FPS_CAP;
	bool m_IsFullScreen;
	glm::vec2 m_WindowsSize;

	bool m_Sync = true;
};