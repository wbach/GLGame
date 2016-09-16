#include "Player.h"
CPlayer::CPlayer()
{
	
}

CPlayer::CPlayer(glm::vec3 pos) :CEntity(pos) {}

CPlayer::CPlayer(glm::vec3 pos, glm::vec3 rot) : CEntity(pos, rot) {}

CPlayer::CPlayer(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : CEntity(pos, rot, scale) {}

void CPlayer::calculateMove(float deltaTime)
{
	checkInputs();
	increaseRotation(0, this->currentTurnSpeed * deltaTime, 0);
	float distance = this->currentSpeed  * deltaTime;
	float dx = static_cast<float>(distance * sin(toRadians(this->getRotation().y)));
	float dz = static_cast<float>(distance * cos(toRadians(this->getRotation().y)));
	//	upwardsSpeed += GRAVITY* deltaTime;	
	incrasePosition(dx, 0, dz);
}

void CPlayer::move(float deltaTime,float terrainHeight)
{
	upwardsSpeed += GRAVITY* deltaTime;
	incrasePosition(0, upwardsSpeed * 0.01, 0);

	float yvalue = 0;
	yvalue = terrainHeight;		
	
	if (getPosition().y < yvalue) {
		upwardsSpeed = 0;
		getReferencedPosition().y = yvalue;
		isInAir = false;
	}

	if (currentSpeed <= 0 && !attacking)
		action = IDLE;
	else if (currentSpeed >0 && !attacking)
		action = RUN;
	else if (attacking)
		action = ATTACK;



	switch (action) {
	case IDLE:
	//	setCurrentFBXmesh(0);
		break;
	case RUN:
	//	setCurrentFBXmesh(1);
		break;
	case ATTACK:
	//	setCurrentFBXmesh(2);
		break;
	}
}

void CPlayer::jump()
{
	if (!isInAir) {
		this->upwardsSpeed = JUMP_POWER;
		isInAir = true;
	}
}

void CPlayer::checkInputs()
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_P]) {
		system("cls");
		printf("Current player position: %.2f, %.2f, %.2f", getPosition().x, getPosition().y, getPosition().z);
	}
	if (state[SDL_SCANCODE_W]) {
		this->currentSpeed = RUN_SPEED;
		//setCurrentFBXmesh(1);
	}
	else if (state[SDL_SCANCODE_S]) {
		this->currentSpeed = -RUN_SPEED;
		//	setCurrentFBXmesh(1);
	}
	else
	{
		this->currentSpeed = 0;
	}
	if (state[SDL_SCANCODE_D]) {
		this->currentTurnSpeed = -TURN_SPEED;
	}
	else if (state[SDL_SCANCODE_A]) {
		this->currentTurnSpeed = TURN_SPEED;
	}
	else {
		this->currentTurnSpeed = 0;
	}
	if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
		attack();
		attacking = true;
	}
	else
		attacking = false;
	if (state[SDL_SCANCODE_SPACE]) {
		jump();
	}
}

void CPlayer::attack()
{
	this->currentSpeed = 0;
}