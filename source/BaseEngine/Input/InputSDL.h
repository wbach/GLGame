#pragma once
#include "Input.h"
#include <SDL2/SDL.h>

class CInputSDL : public CInput
{
public:
	CInputSDL(SDL_Window* sdl_window);
	// Keyboard
	virtual bool GetKeyUp(int i) override;
	virtual bool GetKey(int i) override;
	virtual bool GetKeyDown(int i) override;
	
	//Mouse
	virtual bool GetMouseKeyDown(int key) override;
	virtual bool GetMouseKeyUp(int key) override;
	virtual bool GetMouseKey(int key) override;
	virtual glm::vec2 CalcualteMouseMove() override;
	virtual glm::vec2 GetMousePosition() override;

private:
	int KeyToSDL(int i);

	SDL_Window* m_SdlWindow;	
};