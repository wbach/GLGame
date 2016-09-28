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
	void RenderEntity(const shared_ptr<CEntity>&, CModel& model, const CEntityGeometryPassShader& geomentry_shader) const;
	void Initialize(const glm::mat4& projection_matrix);
	void Render(const shared_ptr<CScene>& scene, const CEntityGeometryPassShader& geomentry_shader) const;
private:

};
