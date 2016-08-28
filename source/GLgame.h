#ifndef GL_GAME_H
#define GL_GAME_H
#pragma once
#include <vector>
#include <thread>
#include "UTIL.h"
#include "Scene.h"
#include "EnitityRenderer.h"
#include "DisplayManager.h"
#include "LoadingShader.h"
#include "GUIRenderer.h"
#include "TerrainRenderer.h"
using namespace std;
const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 10000;
const float FOV = 50;

class CGame {
    LoadingShader loadingShader;

    vector<shared_ptr<CScene>>scenes;
	shared_ptr<CScene> currScene;

	CEntityRenderer entityRenderer;
	CTerrainRenderer terrainRenderer;
    CGUIRenderer guiRenderer;

	glm::vec3 backgroundColour;
	glm::mat4 projectionMatrix;

	bool loading;
public:

	CDisplayManager displayManager;

	CGame() {
		backgroundColour = glm::vec3(0.6,0.6,0.8);
	}
	void initialize();
	void gameLoop();
	void uninitialize();

	void createProjectionMatrix();

    int setCurrentScene(int i);
    void loadScene();
	void initializeScene() ;
	void addScene(shared_ptr<CScene> scene);

	void renderStartSeries();
	float fade(Uint32 deltaTime);
	float fadeIn(Uint32 deltaTime, Uint32 startTime, Uint32 durration);
	float fadeOut(Uint32 deltaTime, Uint32 startTime, Uint32 durration);
};
#endif // !GL_GAME_H


