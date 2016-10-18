#include "Rigidbody.h"
#include "../Utils/Utils.h"
CRigidbody::CRigidbody()
{
	if (m_Mass == 0)
		m_InvMass = 0;
	else
		m_InvMass = 1.f / m_Mass;
}

CRigidbody::CRigidbody(SSphere sphere)
	: m_Colider(sphere)

{
	if (m_Mass == 0)
		m_InvMass = 0;
	else
		m_InvMass = 1.f / m_Mass;
}
CRigidbody::CRigidbody(SAabb aabb)
	: m_Colider(aabb)

{
	if (m_Mass == 0)
		m_InvMass = 0;
	else
		m_InvMass = 1.f / m_Mass;
}

void CRigidbody::ApplyGravity(float delta_time, float gravity)
{
	m_AngularVelocity.y += gravity * delta_time * m_Mass;
}

void CRigidbody::ReflectVelocity(glm::vec3 normal)
{
	glm::vec3  reflected = glm::reflect(m_AngularVelocity, glm::normalize(normal));
	/*glm::vec3 n = glm::normalize(normal);
	float angle = glm::dot(n, m_AngularVelocity);
	glm::vec3 dir = m_AngularVelocity - (2.f * angle)*n;
	m_AngularVelocity = dir ;*/
	//Utils::PrintVector("v : ", reflected);

	m_AngularVelocity = reflected * m_InvMass;//*m_Restitution;
}

void CRigidbody::SetPosition(glm::vec3 position)
{
	m_Colider.SetPosition(position);
}
