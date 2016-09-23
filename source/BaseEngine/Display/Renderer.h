#pragma once

class CRenderer 
{
public:
	virtual void InitializeRenderer() = 0;
	virtual void CalculateFPS() = 0;
	virtual void UninitializeRenderer() = 0;
};