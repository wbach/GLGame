#pragma once
#include <vector>
#include <memory>

namespace GameObjects
{
	enum Type
	{
		ENTITY = 0,
		TERRAIN,
		NONE
	};
}
class CGameObject
{
public:
	CGameObject();
	const GameObjects::Type& GetType();
	const unsigned int& GetId();

	//virtual std::vector<std::shared_ptr<CGameObject>>& GetChildren() {return std::vector<std::shared_ptr<CGameObject>>}

private:
	GameObjects::Type m_Type;

	static unsigned int s_ID;
	unsigned int m_ID;
};