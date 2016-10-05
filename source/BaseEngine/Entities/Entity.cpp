#include "Entity.h"

void CEntity::SetPosition(glm::vec3 position, unsigned int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].position = position;
	CalculateEntityTransformMatrix(i);
}

void CEntity::SetRotation(glm::vec3 rotation, unsigned int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].rotation = rotation;
	CalculateEntityTransformMatrix(i);
}

void CEntity::SetScale(glm::vec3 scale, unsigned int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].scale = scale;
	CalculateEntityTransformMatrix(i);
}

void CEntity::SetTransform(STransform transform, unsigned int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i] = transform;
	CalculateEntityTransformMatrix(i);
}

glm::vec2 CEntity::GetPositionXZ(unsigned int i)
{
//	if (i < 0 || i > m_Transforms.size()) return glm::vec2(0);
	return glm::vec2(m_Transforms[i].position.x, m_Transforms[i].position.z);
}

const glm::vec3& CEntity::GetPosition(unsigned int i)
{
//	if (i < 0 || i > m_Transforms.size()) return glm::vec3(0);
	return m_Transforms[i].position;
}

const glm::vec3& CEntity::GetRotation(unsigned int i)
{
//	if (i < 0 || i > m_Transforms.size()) return glm::vec3(0);
	return m_Transforms[i].rotation;
}

const glm::vec3& CEntity::GetScale(unsigned int i)
{
	if (i < 0 || i > m_Transforms.size()) return glm::vec3(0);
	return m_Transforms[i].scale;
}
glm::vec3& CEntity::GetReferencedPosition(unsigned int i)
{
	//if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return m_Transforms[i].position;
}

glm::vec3& CEntity::GetReferencedRotation(unsigned int i)
{
	//if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return m_Transforms[i].rotation;
}

glm::vec3& CEntity::GetReferencedScale(unsigned int i)
{
	//if (i < 0 || i > transforms.size()) return glm::vec3(0);
	return m_Transforms[i].scale;
}
const STransform& CEntity::GetTransform(unsigned int i)
{
	if (i < 0 || i > m_TransformMatrixes.size()) return STransform();
	return m_Transforms[i];
}

const glm::mat4& CEntity::GetTransformMatrix(unsigned int i)
{
	if (i < 0 || i > m_TransformMatrixes.size()) return glm::mat4(1);
	return m_TransformMatrixes[i];
}

CEntity::CEntity()
	: CEntity(glm::vec3(0))
{
}

CEntity::CEntity(glm::vec3 pos)
	: CEntity(pos, glm::vec3(0))
{

}

CEntity::CEntity(glm::vec3 pos, glm::vec3 rot)
	: CEntity(pos, rot, glm::vec3(1))
{
}

CEntity::CEntity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	m_Name = "No name entity";
	AddTransform(STransform(pos, rot, scale));
}

void CEntity::AddModel(unsigned int model_id)
{
	m_ModelId.push_back(model_id);
}

const int& CEntity::GetModelId(unsigned int i) const
{
	if (i < m_ModelId.size())
	{
		if (i == 0)
			return m_ModelId[m_CurrentModelId];
		return m_ModelId[i];
	}
	else
		return -1;
		
}

void CEntity::Update()
{

}

void CEntity::AddTransform(STransform transform)
{
	m_Transforms.push_back(transform);
	m_TransformMatrixes.push_back(Utils::CreateTransformationMatrix(transform.position, transform.rotation, transform.scale));
}

void CEntity::IncrasePosition(float dx, float dy, float dz, unsigned int index)
{
	this->m_Transforms[index].position.x += dx;
	this->m_Transforms[index].position.y += dy;
	this->m_Transforms[index].position.z += dz;
	CalculateEntityTransformMatrix(index);
}

void CEntity::IncreaseRotation(float dx, float dy, float dz, unsigned int index)
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
void CEntity::CalculateEntityTransformMatrix(unsigned int x)
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
