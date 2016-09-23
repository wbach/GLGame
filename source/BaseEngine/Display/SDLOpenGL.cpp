#include "SDLOpenGL.h"

void CSdlOpenGlApi::CreateWindow(int width, int height)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	unsigned int flags = SDL_WINDOW_OPENGL;
	if (!(m_Window = SDL_CreateWindow("My OpenGL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags)))
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

void CSdlOpenGlApi::ShowCoursor(bool show)
{
	SDL_ShowCursor(show ? SDL_ENABLE : SDL_DISABLE);
}

void CSdlOpenGlApi::SetInput(std::shared_ptr<CInput>& input)
{
	input = std::make_shared<CInputSDL>(m_Window);
}
