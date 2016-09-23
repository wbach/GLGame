#include "InputManager.h"

CInputManager::CInputManager()
: m_Input(nullptr)
{

}

bool CInputManager::GetKeyDown(int key)
{
	return false;
}

bool CInputManager::GetKeyUp(int key)
{
	return false;
}

bool CInputManager::GetKey(int key)
{

	if (m_Input != nullptr)
	{
		return m_Input->GetKey(key);
	}
	return false;
}

bool CInputManager::GetMouseKeyDown(int key)
{
	return false;
}

bool CInputManager::GetMouseKeyUp(int key)
{
	return false;
}

bool CInputManager::GetMouseKey(int key)
{
	if (m_Input != nullptr)
		return m_Input->GetMouseKey(key);
	return false;
}

glm::vec2 CInputManager::GetMousePosition()
{
	if (m_Input != nullptr)
		return m_Input->GetMousePosition();
	return glm::vec2();
}

glm::vec2 CInputManager::CalcualteMouseMove()
{
	if (m_Input != nullptr)
		return m_Input->CalcualteMouseMove();

	return glm::vec2();
}
