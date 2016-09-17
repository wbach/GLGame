#pragma once
#include <vector>
#include <thread>
#include "../Utils/Utils.h"
#include "Scene.h"
#include "../Entities/EnitityRenderer.h"
#include "../Display/DisplayManager.h"
#include "../Shaders/LoadingShader.h"
#include "../GUI/GUIRenderer.h"
#include "../Terrain/TerrainRenderer.h"
using namespace std;

class CGame {
public:
	CGame();

	void Initialize();
	void GameLoop();
	void Uninitialize();

	const CDisplayManager& GetDisplayManager() { return m_DisplayManager; }
	void CreateProjectionMatrix();

    int SetCurrentScene(int i);
    void LoadScene();
	void InitializeScene() ;
	void AddScene(shared_ptr<CScene> scene);

	void RenderStartSeries();
	float Fade(Uint32 delta_time);
	float FadeIn(Uint32 delta_time, Uint32 start_time, Uint32 durration);
	float FadeOut(Uint32 delta_time, Uint32 start_time, Uint32 durration);

private:
	CDisplayManager m_DisplayManager;

	LoadingShader m_LoadingShader;

	vector<shared_ptr<CScene>> m_Scenes;
	shared_ptr<CScene> m_CurrScene;

	CEntityRenderer m_EntityRenderer;
	CTerrainRenderer m_TerrainRenderer;
	CGUIRenderer m_GuiRenderer;

	glm::vec3 m_BackgroundColour;
	glm::mat4 m_ProjectionMatrix;

	bool m_IsLoading;

	glm::vec2 m_WindowSize;

	const float m_NearPlane = 0.1f;
	const float m_FarPlane = 10000;
	const float m_Fov = 50;
};



