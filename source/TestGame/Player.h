#pragma once
#include <SDL2/SDL.h>
#include "../BaseEngine/Entities/Entity.h"
namespace CharacterActions
{
	enum {
		IDLE = 0,
		RUN,
		ATTACK
	};
}

class CPlayer : public CEntity{
	float RUN_SPEED = 20;
	float TURN_SPEED = 160;
	float GRAVITY =-50;
	float JUMP_POWER = 25;
	float currentSpeed = 0;
	float currentTurnSpeed = 0;
	float upwardsSpeed = 0;
	bool isInAir = false;
	int action = CharacterActions::IDLE;
	bool attacking = false;
public:
	CPlayer();
	CPlayer(glm::vec3 pos);
	CPlayer(glm::vec3 pos, glm::vec3 rot);
	CPlayer(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	void calculateMove(float deltaTime);
	void move(float deltaTime, float terrainHeight);
	void setIsInAir(bool is) { isInAir = is; }
	void setUpwardsSpeed(float v) { upwardsSpeed = v; }
	void jump();
	void checkInputs();
	void attack();
	int getAction() { return action; }
};
