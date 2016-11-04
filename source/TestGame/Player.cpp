#include "Player.h"
CPlayer::CPlayer() : CEntity()
{
	
}

CPlayer::CPlayer(CInputManager* input_manager, glm::vec3 pos) : m_InputManager(input_manager), CEntity(pos) {}

CPlayer::CPlayer(CInputManager* input_manager, glm::vec3 pos, glm::vec3 rot) : m_InputManager(input_manager), CEntity(pos, rot) {}

CPlayer::CPlayer(CInputManager* input_manager, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) : m_InputManager(input_manager), CEntity(pos, rot, scale) {}

void CPlayer::calculateMove(float deltaTime)
{
	m_RigidBody.m_Restitution = 0;
	checkInputs(deltaTime);
	IncreaseRotation(0, this->currentTurnSpeed * deltaTime, 0);
	float distance = this->currentSpeed;
	float dx = static_cast<float>(distance * sin(Utils::ToRadians(this->GetRotation().y)));
	float dz = static_cast<float>(distance * cos(Utils::ToRadians(this->GetRotation().y)));

	m_RigidBody.m_AngularVelocity.x = dx;
	m_RigidBody.m_AngularVelocity.z = dz;
}

void CPlayer::move(float deltaTime, float terrainHeight)
{
	if (currentSpeed <= 0 && !attacking)
		action = CharacterActions::IDLE;
	else if (currentSpeed >0 && !attacking)
		action = CharacterActions::RUN;
	else if (attacking)
		action = CharacterActions::ATTACK;

	switch (action)
	{
	case CharacterActions::IDLE:
		m_CurrentModelId = 0;
		break;
	case CharacterActions::RUN:
		m_CurrentModelId = 1;
		break;
	case CharacterActions::ATTACK:
	//	setCurrentFBXmesh(2);
		break;
	}
}

void CPlayer::jump(float deltaTime)
{
	if (m_RigidBody.m_IsGrounded)
	{
		m_RigidBody.m_AngularVelocity.y = JUMP_POWER ;
		m_RigidBody.m_IsGrounded = false;
	}
}

void CPlayer::checkInputs(float deltaTime)
{
	if (m_InputManager == nullptr) return;

	
	if (m_InputManager->GetKey(KeyCodes::P))
	{
		system("cls");
		printf("Current player position: %.2f, %.2f, %.2f", GetWorldPosition().x, GetWorldPosition().y, GetWorldPosition().z);
	}
	if (m_InputManager->GetKey(KeyCodes::W))
	{
		currentSpeed = RUN_SPEED;
	}
	else if (m_InputManager->GetKey(KeyCodes::S))
	{
		currentSpeed = -RUN_SPEED;
	}
	else
	{
		currentSpeed = 0;
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
		jump(deltaTime);
	}
}

void CPlayer::attack()
{
	this->currentSpeed = 0;
}
