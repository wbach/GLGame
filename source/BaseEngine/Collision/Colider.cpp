#include "Colider.h"
#include  "../Utils/Utils.h"
CColider::CColider()
: m_UpdateVector(0)
{
}

CColider::CColider(SSphere sphere)
: m_Sphere(sphere)
, m_ColiderType(ColiderType::SPHERE_COLIDER)
, m_UpdateVector(0)
{
}

CColider::CColider(SAabb aabb)
: m_ColiderType(ColiderType::BOX_COLIDER)
, m_Aabb(aabb)
, m_UpdateVector(0)
{
}

CColider::CColider(std::vector<SFace> faces)
: m_ColiderType(ColiderType::MESH_COLIDER)
, m_Faces(faces)
, m_UpdateVector(0)
{

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

float CColider::GetHeight(const float& x, const float& y, const float& z) const
{
	float height = -1;
	float yy = y + 0.1;
	if (m_ColiderType == ColiderType::MESH_COLIDER)
	{
		for (const SFace& face : m_Faces)
		{			
			glm::vec3 ray_dir(0.f, -1.f, 0.f);			
			
			float h = IntersectTriangle(face.normal, glm::vec3(x, y, z), ray_dir, face.vertexes);
			//Utils::PrintVector("", glm::vec3(h, 0, z));
			if (h > 0)
				if ((yy - h) > height)
				{
					height = yy - h;
				}				
		}
	}
	
	return height;
}

bool CColider::DetectCollision(CColider& colider, std::list<SCollisionInfo>& colision_info) const
{
	switch (m_ColiderType)
	{
	case ColiderType::BOX_COLIDER:
		switch (colider.m_ColiderType)
		{
			case ColiderType::BOX_COLIDER:
				colision_info.push_back(SCollisionInfo());
				return DettectAABBAABB(m_Aabb, colider.m_Aabb, colision_info.back());
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
			colision_info.push_back(SCollisionInfo());
			return DetectSphereSphere(m_Sphere, colider.m_Sphere, colision_info.back());
			break;
		case ColiderType::MESH_COLIDER:
			return DetectFaceSphere(m_Sphere, colider.m_Faces, colision_info);
			break;
		}
		break;
	case ColiderType::MESH_COLIDER:
		switch (colider.m_ColiderType)
		{
		case ColiderType::SPHERE_COLIDER:
			return DetectFaceSphere(colider.m_Sphere, m_Faces, colision_info);
			break;
		}
		break;
	}
	return false;
}

bool CColider::DetectSphereSphere(const SSphere& a, const SSphere& b, SCollisionInfo& colision_info) const
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

bool CColider::DetectFaceSphere(const SSphere& a, const std::vector<SFace>& faces, std::list<SCollisionInfo>& colision_info) const
{
	bool is_col = false;
	for (const SFace& face : faces)
	{
		SCollisionInfo info;
		DetectSphereTriangleCollision(a.position, a.radius, face.vertexes, face.normal, info);
		if (info.collision)
		{
			is_col = true;
			colision_info.push_back(info);
		}
			
	}
	return is_col;
}

bool CColider::DettectAABBAABB(const SAabb & a, const SAabb & b, SCollisionInfo& colision_info) const
{
	if (a.max.x < b.min.x || a.min.x > b.max.x) return false;
	if (a.max.y < b.min.y || a.min.y > b.max.y) return false;
	if (a.max.z < b.min.z || a.min.z > b.max.z) return false;
	return true;
}
float CColider::IntersectTriangle(const glm::vec3& normal, const glm::vec3& rayPos, const glm::vec3& rayDirection, const SFaceVertex& vertexes) const
{
	glm::vec3 v0 = vertexes.v3 - vertexes.v1;
	glm::vec3 v1 = vertexes.v2 - vertexes.v1;
	float d = -1 * (glm::dot(normal, vertexes.v1));
	float t = -(normal.x*rayPos.x + normal.y*rayPos.y + normal.z*rayPos.z + d) / (normal.x*rayDirection.x + normal.y*rayDirection.y + normal.z*rayDirection.z);
	glm::vec3 point = rayPos + t*rayDirection;
	glm::vec3 v2 = point - vertexes.v1;
	float dot00 = glm::dot(v0, v0);
	float dot01 = glm::dot(v0, v1);
	float dot02 = glm::dot(v0, v2);
	float dot11 = glm::dot(v1, v1);
	float dot12 = glm::dot(v1, v2);
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	if ((u >= 0) && (v >= 0) && (u + v < 1)) 
	{
		return t;
	}
	else return -1;
}
bool CColider::DetectSphereTriangleCollision(const glm::vec3& sp, const float& r, const SFaceVertex& vertexes, const glm::vec3& normal, SCollisionInfo& colision_info) const
{
	glm::vec3 n = glm::normalize(normal);

	GLfloat dist1 = IntersectTriangle(-n, sp, n, vertexes);
	GLfloat dist2 = IntersectTriangle(n, sp, -n, vertexes);

	colision_info.collision = false;
	colision_info.normal = n;
	colision_info.updateVector = glm::vec3(0);

	if (dist2 > 0 && dist2 < r) {
		colision_info.normal = n;
		colision_info.collision = true;
		colision_info.updateVector = n*(r - dist2);
	}
	else if (dist1 > 0 && dist1 < r) {
		colision_info.normal = -n;
		colision_info.updateVector = -n*(r - dist1);
		colision_info.collision = true;
		colision_info.collisionY = sp.y + colision_info.updateVector.y;
	}
	return colision_info.collision;
}