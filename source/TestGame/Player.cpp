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
	IncreaseRotation(0, this->currentTurnSpeed * deltaTime, 0);
	float distance = this->currentSpeed  * deltaTime;
	float dx = static_cast<float>(distance * sin(Utils::ToRadians(this->GetRotation().y)));
	float dz = static_cast<float>(distance * cos(Utils::ToRadians(this->GetRotation().y)));
	//	upwardsSpeed += GRAVITY* deltaTime;	
	IncrasePosition(dx, 0, dz);
}

void CPlayer::move(float deltaTime, float terrainHeight)
{
	upwardsSpeed += GRAVITY * deltaTime;
	IncrasePosition(0, upwardsSpeed * 0.01f, 0);

	float yvalue = 0;
	yvalue = terrainHeight;		
	
	if (GetPosition().y < yvalue)
	{
		upwardsSpeed = 0;
		GetReferencedPosition().y = yvalue;
		isInAir = false;
	}

	if (currentSpeed <= 0 && !attacking)
		action = CharacterActions::IDLE;
	else if (currentSpeed >0 && !attacking)
		action = CharacterActions::RUN;
	else if (attacking)
		action = CharacterActions::ATTACK;



	switch (action) {
	case CharacterActions::IDLE:
	//	setCurrentFBXmesh(0);
		break;
	case CharacterActions::RUN:
	//	setCurrentFBXmesh(1);
		break;
	case CharacterActions::ATTACK:
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
		printf("Current player position: %.2f, %.2f, %.2f", GetPosition().x, GetPosition().y, GetPosition().z);
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
