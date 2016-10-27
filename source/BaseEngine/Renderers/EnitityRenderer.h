#pragma once
#include "../Engine/Transform.h"
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "../Entities/Entity.h"
#include "../Engine/Scene.h"
#include "../Shaders/EntityGeometryPassShader.h"
using namespace std;
class CEntityRenderer 
{	
public :
	CEntityRenderer() {}
	void RenderEntity(CScene* scene, CEntity* entity, CModel& model, const CEntityGeometryPassShader& geomentry_shader);
	void RenderEntityRecursive(CScene* scene, CEntity* entity, const CEntityGeometryPassShader& geomentry_shader);
	void Render(CScene* scene, const CEntityGeometryPassShader& geomentry_shader);

	const unsigned int& GetObjectsPerFrame();
	const unsigned int& GetVertexPerFrame();
private:
	unsigned int m_RendererObjectPerFrame = 0;
	unsigned int m_RendererVertixesPerFrame = 0;
	unsigned int m_MaxVerices = 0;
};
