#include "Entity.h"

int CEntity::s_ID = 0;

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
	: m_NormalizedMatrix(1.f)
	, m_RelativeTransformMatrix(1.f)
{	
	m_Id = s_ID++;
	m_Name = "No name entity";
	AddTransform(STransform(pos, rot, scale));
}

void CEntity::SetPosition(glm::vec3 position, unsigned int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].position = position;
	CalculateEntityTransformMatrix(i);
}

void CEntity::SetRotationX(float x, unsigned int index)
{
	m_Transforms[index].rotation.x = x;
	CalculateEntityTransformMatrix(index);
}

void CEntity::SetRotationY(float y, unsigned int index)
{
	m_Transforms[index].rotation.y = y;
	CalculateEntityTransformMatrix(index);
}

void CEntity::SetRotationZ(float z, unsigned int index)
{
	m_Transforms[index].rotation.z = z;
	CalculateEntityTransformMatrix(index);
}

void CEntity::SetRotation(glm::vec3 rotation, unsigned int i)
{
	if (i < 0 || i > m_Transforms.size()) return;
	m_Transforms[i].rotation = rotation;
	CalculateEntityTransformMatrix(i);
}

void CEntity::SetScaleX(float x, unsigned int index)
{
	m_Transforms[index].scale.x = x;
	CalculateEntityTransformMatrix(index);
}

void CEntity::SetScaleY(float y, unsigned int index)
{
	m_Transforms[index].scale.y = y;
	CalculateEntityTransformMatrix(index);
}

void CEntity::SetScaleZ(float z, unsigned int index)
{
	m_Transforms[index].scale.z = z;
	CalculateEntityTransformMatrix(index);
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

const glm::vec3& CEntity::GetLocalPosition(unsigned int i) const
{
//	if (i < 0 || i > m_Transforms.size()) return glm::vec3(0);
	return m_Transforms[i].position;
}

const glm::vec3 CEntity::GetWorldPosition(unsigned int i) const
{
	glm::vec4 pos = m_RelativeTransformMatrix * glm::vec4(m_Transforms[i].position, 1.f);
	return glm::vec3(pos.x, pos.y, pos.z);
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
vector<shared_ptr<CEntity>>& CEntity::GetChildrenEntities()
{ return m_ChildrenEntities;
}
const string CEntity::GetNameWithID() const
{
	string name = m_Name + "__id_e" + std::to_string(m_Id);
	return name;
}
const string CEntity::GetName() const
{ 
	return m_Name;
}
const int& CEntity::GetId()
{
	return m_Id;
}
void CEntity::SetNormalizedSize(const glm::vec3 & v)
{
	m_NormalizedSize = v;
}
const glm::vec3 & CEntity::GetNormalizedSize()
{
	return m_NormalizedSize;
}
void CEntity::SetNormalizedMatrix(const glm::mat4& m)
{
	m_NormalizedMatrix = m;
	CalculateEntityTransformMatrix();
}
void CEntity::RecursiveResetEnities(shared_ptr<CEntity>& entity)
{
	for (shared_ptr<CEntity>& subentity : entity->GetChildrenEntities())
	{
		RecursiveResetEnities(subentity);
		subentity.reset();
	}
}
void CEntity::CleanUp()
{
	for (shared_ptr<CEntity>& entity : m_ChildrenEntities)
	{
		RecursiveResetEnities(entity);
		entity.reset();
	}
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

const glm::mat4 & CEntity::GetRelativeTransformMatrix(unsigned int i)
{
	return m_RelativeTransformMatrix;
}

void CEntity::AddModel(unsigned int model_id, std::string path)
{
	m_ModelId.push_back(model_id);
	m_FullPathFile = path;
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

void CEntity::SetPositionX(float x, unsigned int index)
{
	m_Transforms[index].position.x = x;
	CalculateEntityTransformMatrix(index);
}

void CEntity::SetPositionY(float y, unsigned int index)
{
	m_Transforms[index].position.y = y;
	CalculateEntityTransformMatrix(index);
}

void CEntity::SetPositionZ(float z, unsigned int index)
{
	m_Transforms[index].position.z = z;
	CalculateEntityTransformMatrix(index);
}

void CEntity::IncrasePosition(float dx, float dy, float dz, unsigned int index)
{
	this->m_Transforms[index].position.x += dx;
	this->m_Transforms[index].position.y += dy;
	this->m_Transforms[index].position.z += dz;
	CalculateEntityTransformMatrix(index);
}

void CEntity::IncrasePosition(glm::vec3 v, unsigned int index)
{
	this->m_Transforms[index].position.x += v.x;
	this->m_Transforms[index].position.y += v.y;
	this->m_Transforms[index].position.z += v.z;
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
	if (x < 0 || x > m_Transforms.size()) 
	{
		int i = 0;
		for (STransform transform : m_Transforms)
			m_TransformMatrixes[i++] = Utils::CreateTransformationMatrix(transform.position, transform.rotation, transform.scale);
	}
	else
	{
		m_TransformMatrixes[x] = Utils::CreateTransformationMatrix(m_Transforms[x].position, m_Transforms[x].rotation, m_Transforms[x].scale);

		for (std::shared_ptr<CEntity> entity : m_ChildrenEntities)
		{
			RecursiveSetRelativeTransformMatrix(entity, m_TransformMatrixes[x]);
		}
		m_TransformMatrixes[x] =  m_TransformMatrixes[x] * m_NormalizedMatrix;
	}
	m_TransformsInVao = false;
}

void CEntity::RecursiveSetRelativeTransformMatrix(shared_ptr<CEntity> e , const glm::mat4& parent_matrix)
{
	e->SetRelativeMatrix(parent_matrix);

	for (std::shared_ptr<CEntity> subentity : e->GetChildrenEntities())
	{
		RecursiveSetRelativeTransformMatrix(subentity, parent_matrix * e->GetTransformMatrix());
	}
}

void CEntity::SetRelativeMatrix(const glm::mat4 & parent_matrix)
{
	m_RelativeTransformMatrix = parent_matrix;
}

void CEntity::CalculateFinalTransformMatrix(unsigned int x)
{
	m_FinalTransformMatrix = m_NormalizedMatrix * m_RelativeTransformMatrix * m_TransformMatrixes[x];
}

const glm::mat4 & CEntity::GetFinalTransformMatrix()
{
	return m_FinalTransformMatrix;
}

const glm::mat4 & CEntity::GetNormalizedMatrix()
{
	return m_NormalizedMatrix;
}
