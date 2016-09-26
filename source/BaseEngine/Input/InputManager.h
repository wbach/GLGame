#pragma once
#include "Input.h"
#include <memory>
class CInputManager
{
public:	
	CInputManager();
	void ClearBuffer();

	bool GetKeyDown(int key);
	bool GetKeyUp(int key) ;
	bool GetKey(int key);

	bool GetMouseKeyDown(int key);
	bool GetMouseKeyUp(int key);
	bool GetMouseKey(int key);
	glm::vec2 GetMousePosition();

	glm::vec2 CalcualteMouseMove();

	std::shared_ptr<CInput> m_Input;
private:
	
	int m_InputSource = 0;
};