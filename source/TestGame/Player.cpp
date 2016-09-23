#include "Player.h"
CPlayer::CPlayer()
{
	
}

CPlayer::CPlayer(CInputManager* input_manager, glm::vec3 pos) : m_InputManager(input_manager), CEntity(pos) {}

CPlayer::CPlayer(CInputManager* input_manager, glm::vec3 pos, glm::vec3 rot) : m_InputManager(input_manager), CEntity(pos, rot) {}

CPlayer::CPlayer(CInputManager* input_manager, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : m_InputManager(input_manager), CEntity(pos, rot, scale) {}

void CPlayer::calculateMove(float deltaTime)
{
	checkInputs();
	IncreaseRotation(0, this->currentTurnSpeed * deltaTime, 0);
	float distance = this->currentSpeed  * deltaTime;
	float dx = static_cast<float>(distance * sin(Utils::ToRadians(this->GetRotation().y)));
	float dz = static_cast<float>(distance * cos(Utils::ToRadians(this->GetRotation().y)));
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
	if (m_InputManager == nullptr) return;

	
	if (m_InputManager->GetKey(KeyCodes::P))
	{
		system("cls");
		printf("Current player position: %.2f, %.2f, %.2f", GetPosition().x, GetPosition().y, GetPosition().z);
	}
	if (m_InputManager->GetKey(KeyCodes::W))
	{
		this->currentSpeed = RUN_SPEED;
	}
	else if (m_InputManager->GetKey(KeyCodes::S))
	{
		this->currentSpeed = -RUN_SPEED;
	}
	else
	{
		this->currentSpeed = 0;
	}
	if (m_InputManager->GetKey(KeyCodes::D))
	{
		this->currentTurnSpeed = -TURN_SPEED;
	}
	else if (m_InputManager->GetKey(KeyCodes::A))
	{
		this->currentTurnSpeed = TURN_SPEED;
	}
	else 
	{
		this->currentTurnSpeed = 0;
	}
	if (m_InputManager->GetMouseKey(KeyCodes::LMOUSE))
	{
		attack();
		attacking = true;
	}
	else
		attacking = false;

	if (m_InputManager->GetKey(KeyCodes::SPACE))
	{
		jump();
	}
}

void CPlayer::attack()
{
	this->currentSpeed = 0;
}
