#include "Colider.h"
#include  "../Utils/Utils.h"
CColider::CColider()
{
}

CColider::CColider(SSphere sphere)
{
	m_ColiderType = ColiderType::SPHERE_COLIDER;
	m_Sphere = sphere;
}

CColider::CColider(SAabb aabb)
{
	m_ColiderType = ColiderType::BOX_COLIDER;
	m_Aabb = aabb;
}

void CColider::SetPosition(glm::vec3 position)
{
	switch (m_ColiderType)
	{
	case ColiderType::BOX_COLIDER:
		m_Aabb.position = position;
		break;
	case ColiderType::SPHERE_COLIDER:
		m_Sphere.position = position;
		break;
	}
}

const glm::vec3& CColider::GetPosition() const
{
	switch (m_ColiderType)
		{
		case ColiderType::BOX_COLIDER:
			return m_Aabb.position;
			break;
		case ColiderType::SPHERE_COLIDER:
			return m_Sphere.position;
			break;
		}
	return m_Sphere.position;
}

bool CColider::DetectCollision(CColider& colider)
{
	return DetectSphereSphere(m_Sphere, colider.m_Sphere);
	switch (m_ColiderType)
	{
	case ColiderType::BOX_COLIDER:
		switch (colider.m_ColiderType)
		{
			case ColiderType::BOX_COLIDER:
				return DettectAABBAABB(m_Aabb, colider.m_Aabb);
				break;
			case ColiderType::SPHERE_COLIDER:
				//return DetectSphereSphere(m_Sphere, colider.m_Sphere);
				break;

		}
		break;
	case ColiderType::SPHERE_COLIDER:
		switch (colider.m_ColiderType)
		{
		case ColiderType::SPHERE_COLIDER:
			return DetectSphereSphere(m_Sphere, colider.m_Sphere);
			break;
		}
		break;
	}
	return false;
}

bool CColider::DetectSphereSphere(const SSphere& a, const SSphere& b) const
{
	
	float aa =	(b.position.x - a.position.x)*(b.position.x - a.position.x) +
				(b.position.y - a.position.y)*(b.position.y - a.position.y) +
				(b.position.z - a.position.z)*(b.position.z - a.position.z);
	float r = a.radius + b.radius;
	r *= r;
	
	if (aa <= r)
		return true;		
	else

		return false;
}

bool CColider::DettectAABBAABB(const SAabb & a, const SAabb & b) const
{
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
	if (a.max.z < b.min.z || a.min.z > b.max.z) return false;
	return true;
}
