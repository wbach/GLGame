#include "DisplayManager.h"

int CDisplayManager::Initialize(int api, int renderer, int w, int h)
{
	switch (api) 
	{
	case API::SDL2:
		m_Api = std::make_shared<CSdlOpenGlApi>();
		break;
	}

	if (m_Api == nullptr) return -1;
	m_Api->CreateWindow(w, h);

	switch (renderer)
	{
	case Renderer::OPENGL:
		m_Renderer = std::make_shared<COpenGLRenderer>();
		break;
	}
	if (m_Renderer == nullptr) return -1;
	m_Renderer->InitializeRenderer();


	m_WindowsSize.x = static_cast<float>(w);
	m_WindowsSize.y = static_cast<float>(h);

	m_FPS_CAP = 999;
	m_LastFrameTime = 0;
	return 0;
}

void CDisplayManager::Update()
{
    CalculateFPS();
	float current_frame_time = GetCurrentTime();
	m_Delta = (current_frame_time - m_LastFrameTime) ;
	m_LastFrameTime = current_frame_time;

	if (m_Api != nullptr)
		m_Api->UpdateWindow();
}
const float CDisplayManager::GetCurrentTime()
{
	return static_cast<float>(SDL_GetTicks());
}
void CDisplayManager::Uninitialize()
{
	if (m_Renderer != nullptr)
		m_Renderer->UninitializeRenderer();
	if (m_Api != nullptr)
		m_Api->CleanUp();
}
void CDisplayManager::CalculateFPS()
{
	m_FrameCount++;

	m_CurrentTime = SDL_GetTicks();

	int time_interval = m_CurrentTime - m_PreviousTime;

	if(time_interval > 1000)
	{
		m_Fps = m_FrameCount / (time_interval / 1000.0f);
		m_PreviousTime = m_CurrentTime;
		m_FrameCount = 0;
	}
}
const int CDisplayManager::GetFps()
{
	return (int) m_Fps;
}
void CDisplayManager::SetFullScreen()
{
	m_IsFullScreen = !m_IsFullScreen;
	
	if (m_Api != nullptr)
		m_Api->SetFullScreen(m_IsFullScreen);
}

const glm::vec2& CDisplayManager::GetWindowSize()
{
	return m_WindowsSize;
}

void CDisplayManager::ShowCoursor(bool show)
{
	if (m_Api != nullptr)
		m_Api->ShowCoursor(show);
}

bool CDisplayManager::CheckActiveWindow()
{
	if (m_Api != nullptr)
		return m_Api->CheckActiveWindow();
	return false;
}

void CDisplayManager::SetInput(std::shared_ptr<CInput>& input)
{
	if (m_Api != nullptr)
		m_Api->SetInput(input);
}
