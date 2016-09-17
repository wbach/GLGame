#pragma once
#include "../Engine/Transform.h"
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Entity.h"
#include "EntityShader.h"
#include "../Engine/Scene.h"
using namespace std;
class CEntityRenderer 
{	
public :
	CEntityRenderer() {}
	void RenderEntity(shared_ptr<CEntity>&, CModel& model);
	void Initialize(const glm::mat4& projection_matrix);
	void Uninitialize();
	void Render(shared_ptr<CScene>& scene);
private:
	EntityShader m_EntityShader;
	GLuint m_Vao, m_Ivbo, m_VboId, m_VboTextId;
};
