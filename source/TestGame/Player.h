#pragma once
#include "../BaseEngine/Entities/Entity.h"
#include "../BaseEngine/Input/InputManager.h"
namespace CharacterActions
{
	enum {
		IDLE = 0,
		RUN,
		ATTACK
	};
}

class CPlayer : public CEntity
{	
public:
	CPlayer();
	CPlayer(CInputManager* input_manager, glm::vec3 pos);
	CPlayer(CInputManager* input_manager, glm::vec3 pos, glm::vec3 rot);
	CPlayer(CInputManager* input_manager, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	void calculateMove(float deltaTime);
	void move(float deltaTime, float terrainHeight);
	void setUpwardsSpeed(float v) { upwardsSpeed = v; }
	void jump(float deltaTime);
	void checkInputs(float deltaTime);
	void attack();
	int getAction() { return action; }
private:
	float RUN_SPEED = Utils::KmToMs(15.f);
	float TURN_SPEED = 160;
	float GRAVITY = -50;
	float JUMP_POWER = 25;
	float currentSpeed = 0;
	float currentTurnSpeed = 0;
	float upwardsSpeed = 0;	
	int action = CharacterActions::IDLE;
	bool attacking = false;
	CInputManager* m_InputManager;
};
