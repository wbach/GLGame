#include "Entity.h"

void CEntity::SetPosition(glm::vec3 position, int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].position = position;
	CalculateEntityTransformMatrix(i);
}

void CEntity::SetRotation(glm::vec3 rotation, int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].rotation = rotation;
	CalculateEntityTransformMatrix(i);
}

void CEntity::SetScale(glm::vec3 scale, int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].scale = scale;
	CalculateEntityTransformMatrix(i);
}

void CEntity::SetTransform(STransform transform, int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i] = transform;
	CalculateEntityTransformMatrix(i);
}

glm::vec2 CEntity::GetPositionXZ(int i)
{
	if (i < 0 || i > m_Transforms.size()) return glm::vec2(0);
	return glm::vec2(m_Transforms[i].position.x, m_Transforms[i].position.z);
}

const glm::vec3& CEntity::GetPosition(int i)
{
	if (i < 0 || i > m_Transforms.size()) return glm::vec3(0);
	return m_Transforms[i].position;
}

const glm::vec3& CEntity::GetRotation(int i)
{
	if (i < 0 || i > m_Transforms.size()) return glm::vec3(0);
	return m_Transforms[i].rotation;
}

const glm::vec3& CEntity::GetScale(int i)
{
	if (i < 0 || i > m_Transforms.size()) return glm::vec3(0);
	return m_Transforms[i].scale;
}
glm::vec3& CEntity::GetReferencedPosition(int i)
{
	//if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return m_Transforms[i].position;
}

glm::vec3& CEntity::GetReferencedRotation(int i)
{
	//if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return m_Transforms[i].rotation;
}

glm::vec3& CEntity::GetReferencedScale(int i)
{
	//if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return m_Transforms[i].scale;
}
const STransform& CEntity::GetTransform(int i)
{
	if (i < 0 || i > m_TransformMatrixes.size()) return STransform();
	return m_Transforms[i];
}

const glm::mat4& CEntity::GetTransformMatrix(int i)
{
	if (i < 0 || i > m_TransformMatrixes.size()) return glm::mat4(1);
	return m_TransformMatrixes[i];
}

CEntity::CEntity()
{
	AddTransform(STransform());
}

CEntity::CEntity(glm::vec3 pos)
{
	AddTransform(STransform(pos));
}

CEntity::CEntity(glm::vec3 pos, glm::vec3 rot)
{
	AddTransform(STransform(pos, rot));
}

CEntity::CEntity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	AddTransform(STransform(pos, rot, scale));
}

void CEntity::AddTransform(STransform transform)
{
	m_Transforms.push_back(transform);
	m_TransformMatrixes.push_back(Utils::CreateTransformationMatrix(transform.position, transform.rotation, transform.scale));
}

void CEntity::IncrasePosition(float dx, float dy, float dz, int index)
{
	this->m_Transforms[index].position.x += dx;
	this->m_Transforms[index].position.y += dy;
	this->m_Transforms[index].position.z += dz;
	CalculateEntityTransformMatrix(index);
}

void CEntity::IncreaseRotation(float dx, float dy, float dz, int index)
{
	this->m_Transforms[index].rotation.x += dx;
	this->m_Transforms[index].rotation.y += dy;
	this->m_Transforms[index].rotation.z += dz;
	CalculateEntityTransformMatrix(index);
}
void CEntity::AddSubbEntity(shared_ptr<CEntity> e)
{
	m_ChildrenEntities.push_back(e);
}
void CEntity::CalculateEntityTransformMatrix(int x)
{
	if (x < 0 || x > m_Transforms.size()) {
		int i = 0;
		for (STransform transform : m_Transforms)
			m_TransformMatrixes[i++] = Utils::CreateTransformationMatrix(transform.position, transform.rotation, transform.scale);
	}
	else
	{
		m_TransformMatrixes[x] = Utils::CreateTransformationMatrix(m_Transforms[x].position, m_Transforms[x].rotation, m_Transforms[x].scale);
	}
}
