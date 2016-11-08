#pragma once
#include "Hero.h"
#include "../BaseEngine/Input/InputManager.h"

template<class T>
class CPlayer : virtual public T
{
public:
	CPlayer() : CEntity(){}
	CPlayer(CInputManager* input_manager, glm::vec3 pos) : m_InputManager(input_manager), CEntity(pos) {}
	CPlayer(CInputManager* input_manager, glm::vec3 pos, glm::vec3 rot) : m_InputManager(input_manager), CEntity(pos, rot) {}
	CPlayer(CInputManager* input_manager, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : m_InputManager(input_manager), CEntity(pos, rot, scale) {}

	virtual void Move(const float& delta_time) override
	{
		CheckInputs();
		CHero::Move(delta_time);		
	}
	void CheckInputs()
	{
		if (m_InputManager == nullptr) return;

		if (m_InputManager->GetKey(KeyCodes::P))
		{
			system("cls");
			printf("Current player position: %.2f, %.2f, %.2f", GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z);
		}
		bool move_key_pres = false;
		if (m_InputManager->GetKey(KeyCodes::W))
		{
			m_CharacterStats.m_CurrentMoveSpeed = m_CharacterStats.m_RunSpeed;
			SetAction(CharacterActions::RUN);
			move_key_pres = true;
		}
		else if (m_InputManager->GetKey(KeyCodes::S))
		{
			m_CharacterStats.m_CurrentMoveSpeed = -m_CharacterStats.m_RunSpeed;
			SetAction(CharacterActions::RUN);
			move_key_pres = true;
		}else if (m_InputManager->GetMouseKey(KeyCodes::LMOUSE))
		{
			SetAction(CharacterActions::ATTACK_1);
		}
		else if (m_InputManager->GetMouseKey(KeyCodes::RMOUSE))
		{
			SetAction(CharacterActions::ATTACK_2);
		}
		else if (m_InputManager->GetKey(KeyCodes::Z))
		{
			SetAction(CharacterActions::ATTACK_3);
		}
		else
		{
			SetAction(CharacterActions::IDLE);
		}

		if(!move_key_pres)
			m_CharacterStats.m_CurrentMoveSpeed = 0;

		if (m_InputManager->GetKey(KeyCodes::D))
		{
			m_CharacterStats.m_CurrentTurnSpeed = -m_CharacterStats.m_TurnSpeed;
		}
		else if (m_InputManager->GetKey(KeyCodes::A))
		{
			m_CharacterStats.m_CurrentTurnSpeed = m_CharacterStats.m_TurnSpeed;
		}
		else
		{
			m_CharacterStats.m_CurrentTurnSpeed = 0;
		}


		

		if (m_InputManager->GetKey(KeyCodes::SPACE))
		{
			Jump();
		}
	}
	CInputManager* m_InputManager;
};