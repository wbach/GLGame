#pragma once
#include "GameCharacter.h"

class CHero : virtual public CGameCharacter
{
public:
	virtual void Create(CScene* scene);
	virtual void Move(const float& delta_time);
	void Jump();	
	CharacterActions::Type GetAction() { return m_Action; }
	void SetAction(CharacterActions::Type action);
protected:
	void SetModelId(CharacterActions::Type);

	int m_Lvl = 1;
	long int m_GeneralExp = 0;
	long int m_MeleExp = 0;
	long int m_DistanceExp = 0;
	long int m_AbilityExp = 0;
	long int m_StaminaExp = 0;

	unsigned int m_ModelsIds[CharacterActions::COUNT];
	std::string m_Models[CharacterActions::COUNT];
	CharacterActions::Type m_LastAction = CharacterActions::IDLE;
};