#pragma once
#include "../Input/Input.h"
#include <memory>
class CApi 
{
public:
	virtual void CreateWindow(int width, int height) = 0;
	virtual void CleanUp() = 0;
	virtual void UpdateWindow() = 0;
	virtual void SetFullScreen(bool full_screen) = 0;
	virtual bool CheckActiveWindow() = 0;
	virtual void ShowCoursor(bool show) = 0;
	virtual void SetInput(std::shared_ptr<CInput>&) = 0;
};