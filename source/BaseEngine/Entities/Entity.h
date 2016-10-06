#pragma once
#include <string>
#include "glm/glm.hpp"
#include "../Engine/Transform.h"
#include "../Utils/Utils.h"
#include <vector>
#include <memory>
using namespace std;
class CEntity
{	
public:		
	CEntity();
	CEntity(glm::vec3 pos);
	CEntity(glm::vec3 pos, glm::vec3 rot);
	CEntity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);	
	
	void AddModel(unsigned int model_id);
	const int& GetModelId(unsigned int i = 0) const;

	void Update();

	void AddSubbEntity(shared_ptr<CEntity> e);
	void IncrasePosition(float dx, float dy, float dz, unsigned int index = 0);
	void IncreaseRotation(float dx, float dy, float dz, unsigned int index = 0);
	void CalculateEntityTransformMatrix(unsigned int x = 0);
	void AddTransform(STransform transform);
	
	void SetPositionX(float x, unsigned int index = 0);
	void SetPositionY(float y, unsigned int index = 0);
	void SetPositionZ(float z, unsigned int index = 0);
	void SetPosition(glm::vec3 position, unsigned int i = 0);

	void SetRotationX(float x, unsigned int index = 0);
	void SetRotationY(float y, unsigned int index = 0);
	void SetRotationZ(float z, unsigned int index = 0);
	void SetRotation(glm::vec3 rotation, unsigned int i = 0);

	void SetScaleX(float x, unsigned int index = 0);
	void SetScaleY(float y, unsigned int index = 0);
	void SetScaleZ(float z, unsigned int index = 0);
	void SetScale(glm::vec3 scale, unsigned int i = 0);
	void SetTransform(STransform transform, unsigned int i = 0);

	glm::vec2 GetPositionXZ(unsigned int i = 0);
	const glm::vec3& GetPosition(unsigned int i = 0);
	const glm::vec3& GetRotation(unsigned int i = 0);
	const glm::vec3& GetScale(unsigned int i = 0);

	const STransform& GetTransform(unsigned int i = 0);
	const glm::mat4& GetTransformMatrix(unsigned int i = 0);
	vector<glm::mat4>& GetTransformMatrixes() { return m_TransformMatrixes; }

	glm::vec3& GetReferencedPosition(unsigned int i = 0);
	glm::vec3& GetReferencedRotation(unsigned int i = 0);
	glm::vec3& GetReferencedScale(unsigned int i = 0);

	vector<shared_ptr<CEntity>>& GetChildrenEntities() { return m_ChildrenEntities; }
	
	void SetName(std::string name) { m_Name = name; }
	const string GetName() const;
	const int& GetId();
protected:
	vector<glm::mat4>			m_TransformMatrixes;
	vector<STransform>			m_Transforms;
	vector<shared_ptr<CEntity>> m_ChildrenEntities;

	string m_Name;
	int m_CurrentModelId = 0;
	std::vector<int> m_ModelId;

	int m_Id;
	static int s_ID;
};
