#pragma once
#include <iostream>
#include "glm/glm.hpp"
namespace KeyCodes
{
	enum
	{
		Q = 0x01,
		W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		LCTRL, ENTER, LMOUSE, RMOUSE, SPACE
	};
}
class CInput 
{
public:
	virtual bool GetKeyDown(int i) = 0;
	virtual bool GetKeyUp(int i) = 0;
	virtual bool GetKey(int i) = 0;

	virtual bool GetMouseKeyDown(int key) = 0;
	virtual bool GetMouseKeyUp(int key) = 0;
	virtual bool GetMouseKey(int key) = 0;
	virtual glm::vec2 CalcualteMouseMove() = 0;
	virtual glm::vec2 GetMousePosition() = 0;
};