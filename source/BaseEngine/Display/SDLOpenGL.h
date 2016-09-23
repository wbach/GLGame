#pragma once
#include "Api.h"
#include <SDL2/SDL.h>
#include "../Input/InputSDL.h"
class CSdlOpenGlApi : public CApi
{
public:
	virtual void CreateWindow(int width, int height) override;
	virtual void CleanUp() override;
	virtual void UpdateWindow() override;
	virtual void SetFullScreen(bool full_screen) override;
	virtual bool CheckActiveWindow() override;
	virtual void ShowCoursor(bool show) override;
	virtual void SetInput(std::shared_ptr<CInput>&) override;
private:
	SDL_GLContext m_GlContext;
	SDL_Window* m_Window;	
};