#pragma once
#include <memory>
#include "glm/glm.hpp"
namespace ColiderType 
{
	enum Type
	{
		NONE_COLIDER = 0,
		BOX_COLIDER,
		MESH_COLIDER,
		COlISION_MESH_COLIDER,
		SPHERE_COLIDER
	};
}
struct SAabb
{
	glm::vec3 min;
	glm::vec3 max;
	glm::vec3 position;
	SAabb() {}
	SAabb(glm::vec3 min, glm::vec3 max) 
		: min(min)
		, max(max)
	{}
};
struct SSphere
{
	glm::vec3 position = glm::vec3(0);
	float radius = 1.f;
	SSphere() {}
	SSphere(glm::vec3 pos, float r) { position = pos; radius = r; }
};
class CColider
{
public:
	CColider();
	CColider(SSphere sphere);
	CColider(SAabb aabb);

	void SetPosition(glm::vec3 position);	
	const glm::vec3& GetPosition() const;
	bool DetectCollision(CColider& colider);
	bool DetectSphereSphere(const SSphere& a, const SSphere& b) const;
	bool DettectAABBAABB(const SAabb& a, const SAabb& b) const;
	
	SAabb m_Aabb;
	SSphere m_Sphere;
	ColiderType::Type m_ColiderType = ColiderType::NONE_COLIDER;
};
