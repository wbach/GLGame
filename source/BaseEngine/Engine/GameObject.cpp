#include "GameObject.h"
unsigned int CGameObject::s_ID = 0;

CGameObject::CGameObject()
{
	m_ID = s_ID++;
	m_Type = GameObjects::NONE;
}

const GameObjects::Type & CGameObject::GetType()
{
	return m_Type;
}

const unsigned int & CGameObject::GetId()
{
	return m_ID;
}
