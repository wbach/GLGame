#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H
#pragma once
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Transform.h"
#include "Entity.h"
#include "EntityShader.h"
#include "Scene.h"
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
#endif // !ENTITY_RENDERER_H
