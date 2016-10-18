#pragma once
#include "../Collision/Colider.h"
#include <math.h>
#include <algorithm>
class CRigidbody
{
public:	
	CRigidbody();
	CRigidbody(SSphere sphere);
	CRigidbody(SAabb sphere);
	CColider m_Colider;

	void ApplyGravity(float delta_time, float gravity);
	void ReflectVelocity(glm::vec3 normal);
	void SetPosition(glm::vec3 position);

	glm::vec3 m_AngularVelocity = glm::vec3(0);
	glm::vec3 m_CenterOfMass = glm::vec3(0);

	float m_Mass = 10.f;
	float m_InvMass;
	bool IsUseGravity = true;

	float m_Restitution = 0.7f;
	float m_Magnitude = 0.f;
};