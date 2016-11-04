#pragma once
#include "../Loader/Face.h"
#include <memory>
#include <vector>
#include <list>

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
};


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
	CColider(std::vector<SFace> faces);

	void SetPosition(glm::vec3 position);	
	const glm::vec3& GetPosition() const;
	float GetHeight(const float& x, const float& y, const float& z) const;
	bool DetectCollision(CColider& colider, std::list<SCollisionInfo>& colision_info) const;
	
	SAabb m_Aabb;
	SSphere m_Sphere;
	ColiderType::Type m_ColiderType = ColiderType::NONE_COLIDER;

	glm::vec3 m_UpdateVector;
	float m_MinYPos;
private:
	bool DetectSphereSphere(const SSphere& a, const SSphere& b, SCollisionInfo& colision_info) const;
	bool DetectFaceSphere(const SSphere& a, const std::vector<SFace>& m_Faces, std::list<SCollisionInfo>& colision_info) const;
	bool DettectAABBAABB(const SAabb& a, const SAabb& b, SCollisionInfo& colision_info) const;

	bool DetectSphereTriangleCollision(const glm::vec3& sp, const float& r, const SFaceVertex& vertexes, const glm::vec3& normal, SCollisionInfo& colision_info) const;
	float IntersectTriangle(const glm::vec3& normal, const glm::vec3& rayPos, const glm::vec3& rayDirection, const SFaceVertex& vertexes) const;

	std::vector<SFace> m_Faces;
};
