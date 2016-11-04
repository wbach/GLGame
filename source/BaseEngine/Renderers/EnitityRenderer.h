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
	void RenderEntity(CEntity* entity, CModel& model, const CEntityGeometryPassShader& geomentry_shader);
	void RenderEntityRecursive(CScene* scene, CEntity* entity, const CEntityGeometryPassShader& geomentry_shader);
	void Render(CScene* scene, const CEntityGeometryPassShader& geomentry_shader);
	void RenderModel(CModel& model, const CEntityGeometryPassShader& geomentry_shader,
		const int& transform_matrixes_size = 1,
		const std::vector<glm::mat4>& transform_matrixes = std::vector<glm::mat4>(),
		const glm::mat4& relative_matrix = glm::mat4(1.0f)
	);

	const unsigned int& GetObjectsPerFrame();
	const unsigned int& GetVertexPerFrame();
private:
	unsigned int m_RendererObjectPerFrame = 0;
	unsigned int m_RendererVertixesPerFrame = 0;
	unsigned int m_MaxVerices = 0;
};
