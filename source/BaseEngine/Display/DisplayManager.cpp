#include "DisplayManager.h"

int CDisplayManager::Initialize(int w, int h)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	unsigned int flags = SDL_WINDOW_OPENGL;
	if (!(m_Window = SDL_CreateWindow("My OpenGL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags)))
	{
		cout << "failed to create window\n";
		exit(-1);
	}
	if (!(m_GlContext = SDL_GL_CreateContext(m_Window)))
	{
		cout << "failed to create OpenGL context\n";
		exit(-1);
	}

	GLint glew_init_result = glewInit();
	if (glew_init_result != GLEW_OK)
	{
		printf("ERROR: %s\n", glewGetErrorString(glew_init_result));
		exit(-1);
	}
	printf("GL version: %s\n\n", glGetString(GL_VERSION));

	m_WindowsSize.x = static_cast<float>(w);
	m_WindowsSize.y = static_cast<float>(h);

	m_FPS_CAP = 999;

	return 0;
}

void CDisplayManager::Update()
{
    CalculateFPS();
	float current_frame_time= GetCurrentTime();
	m_Delta = (current_frame_time - m_LastFrameTime) / 1000.0f;
	SDL_GL_SwapWindow(m_Window);
}
const float CDisplayManager::GetCurrentTime()
{
	return static_cast<float>(SDL_GetTicks());
}
void CDisplayManager::Uninitialize()
{
	SDL_GL_DeleteContext(m_GlContext);
	SDL_Quit();
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
	if (m_IsFullScreen) {
		SDL_SetWindowFullscreen(m_Window, SDL_TRUE);
	}else
		SDL_SetWindowFullscreen(m_Window, SDL_FALSE);
}

const glm::vec2& CDisplayManager::GetWindowSize()
{
	return m_WindowsSize;
}
