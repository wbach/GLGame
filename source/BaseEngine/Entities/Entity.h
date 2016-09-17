#pragma once

#include <string>
#include "glm/glm.hpp"
#include "../Engine/Transform.h"
#include "../Utils/Utils.h"
#include <vector>
#include <memory>
using namespace std;
class CEntity {	
public:	
	string m_Name;
	int m_ModelId;
	
	CEntity();
	CEntity(glm::vec3 pos);
	CEntity(glm::vec3 pos, glm::vec3 rot);
	CEntity(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);	
	
	void AddSubbEntity(shared_ptr<CEntity> e);
	void IncrasePosition(float dx, float dy, float dz, int index = 0);
	void IncreaseRotation(float dx, float dy, float dz, int index = 0);
	void CalculateEntityTransformMatrix(int x = 0);
	void AddTransform(STransform transform);
	
	void SetPosition(glm::vec3 position, int i = 0);
	void SetRotation(glm::vec3 rotation, int i = 0);
	void SetScale(glm::vec3 scale, int i = 0);
	void SetTransform(STransform transform, int i = 0);

	glm::vec2 GetPositionXZ(int i = 0);
	const glm::vec3& GetPosition(int i = 0);
	const glm::vec3& GetRotation(int i = 0);
	const glm::vec3& GetScale(int i = 0);
	const STransform& GetTransform(int i = 0);
	const glm::mat4& GetTransformMatrix(int i = 0);
	vector<glm::mat4>& GetTransformMatrixes() { return m_TransformMatrixes; }
	glm::vec3& GetReferencedPosition(int i = 0);
	glm::vec3& GetReferencedRotation(int i = 0);
	glm::vec3& GetReferencedScale(int i = 0);
	vector<shared_ptr<CEntity>>& GetChildrenEntities() { return m_ChildrenEntities; }

private:
	vector<glm::mat4> m_TransformMatrixes;
	vector<STransform> m_Transforms;
	vector<shared_ptr<CEntity>> m_ChildrenEntities;
};
