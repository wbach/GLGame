#include "SDLOpenGL.h"

void CSdlOpenGlApi::CreateWindow(std::string window_name, int width, int height)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	unsigned int flags = SDL_WINDOW_OPENGL;// | SDL_WINDOW_BORDERLESS;
	if (!(m_Window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags)))
	{
	//	cout << "failed to create window\n";
		exit(-1);
	}
	if (!(m_GlContext = SDL_GL_CreateContext(m_Window)))
	{
	//	cout << "failed to create OpenGL context\n";
		exit(-1);
	}
}

void CSdlOpenGlApi::CleanUp()
{
	SDL_GL_DeleteContext(m_GlContext);
	SDL_Quit();
}

void CSdlOpenGlApi::UpdateWindow()
{
	SDL_GL_SwapWindow(m_Window);
}
void CSdlOpenGlApi::SetFullScreen(bool full_screen)
{
	if (full_screen)
		SDL_SetWindowFullscreen(m_Window, SDL_TRUE);
	else
		SDL_SetWindowFullscreen(m_Window, SDL_FALSE);
}

bool CSdlOpenGlApi::CheckActiveWindow()
{
	if ((SDL_GetWindowFlags(m_Window) & SDL_WINDOW_INPUT_FOCUS))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CSdlOpenGlApi::ShowCursor(bool show)
{
	SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
}

void CSdlOpenGlApi::SetInput(std::shared_ptr<CInput>& input)
{
	input = std::make_shared<CInputSDL>(m_Window);
}

double CSdlOpenGlApi::GetTime()
{
	return  static_cast<double>(SDL_GetTicks() / 1000.0);
}

void CSdlOpenGlApi::SetCursorPosition(int x, int y)
{
	SDL_WarpMouseInWindow(m_Window, x, y);
}

void CSdlOpenGlApi::BeginFrame()
{
	m_StartTime = SDL_GetTicks();
}

void CSdlOpenGlApi::LockFps(float fps)
{
	if (static_cast<Uint32>(1000.0f / fps ) > SDL_GetTicks() - m_StartTime)  SDL_Delay(static_cast<Uint32>(1000.0f / fps ) - (SDL_GetTicks() - m_StartTime));
}

int CSdlOpenGlApi::PeekMessage()
{	
	BeginFrame();
	while (SDL_PollEvent(&m_Event))
	{
		switch (m_Event.type)
		{
			case SDL_QUIT: return ApiMessages::QUIT;
			case SDL_MOUSEBUTTONDOWN:

				break;
			case SDL_KEYDOWN:
				switch (m_Event.key.keysym.sym)
				{
				case SDLK_F9: break; // m_DisplayManager.SetFullScreen(); 
				case SDLK_ESCAPE: return ApiMessages::QUIT;
				}
				break;
			case SDL_FINGERDOWN:
			{
				//cout << "Touch " << endl;
			}
		}
	}
	return 0;
}
