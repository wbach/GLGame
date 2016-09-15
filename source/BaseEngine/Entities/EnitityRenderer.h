#pragma once

#include "../Engine/Transform.h"
#include <string>
#include <vector>
#include "glm/glm.hpp"

#include "Entity.h"
#include "EntityShader.h"
#include "../Engine/Scene.h"
using namespace std;
class CEntityRenderer {
	EntityShader entityShader;
	GLuint VAO, ivbo, vboId, vboTextId;
public :
	CEntityRenderer() {  }
	void renderEntity(shared_ptr<CEntity>, CModel &model);
	void initialize(glm::mat4 projectionMatrix);
	void uninitialize();
	void render(shared_ptr<CScene>scene);
};
