#pragma once
#include <fstream>
#include <vector>
#include <thread>
#include "../Utils/Utils.h"
#include "Scene.h"
#include "../Display/DisplayManager.h"
#include "../Shaders/LoadingShader.h"
#include "../GUI/GUIRenderer.h"
#include "../Input/InputManager.h"
#include "../Renderers/MasterRenderer.h"
using namespace std;

class CGame {
public:
	CGame();

	void Initialize();
	void GameLoop();
	void Uninitialize();

	CInputManager& GetInputManager() { return m_InputManager; }
	CDisplayManager& GetDisplayManager() { return m_DisplayManager; }
	void CreateProjectionMatrix();

    int SetCurrentScene(int i);
    void LoadScene();
	void InitializeScene() ;
	void AddScene(shared_ptr<CScene> scene);

	void RenderStartSeries();
	float Fade(Uint32 delta_time);
	float FadeIn(Uint32 delta_time, Uint32 start_time, Uint32 durration);
	float FadeOut(Uint32 delta_time, Uint32 start_time, Uint32 durration);

	int ReadConfiguration(string file_name);
private:
	CDisplayManager m_DisplayManager;
	CInputManager m_InputManager;

	LoadingShader m_LoadingShader;

	vector<shared_ptr<CScene>> m_Scenes;
	shared_ptr<CScene> m_CurrScene;

	CGUIRenderer m_GuiRenderer;
	CMasterRenderer m_MasterRenderer;

	glm::vec3 m_BackgroundColour;
	glm::mat4 m_ProjectionMatrix;

	float m_ViewDistance;
	float m_GrassViewDistance;

	bool m_IsShadows;
	float m_ShadowMapSize;

	float m_WaterQuality;


	bool m_IsLoading;
	bool m_IsFullScreen;	

	glm::vec2 m_WindowSize;
	string m_WindowName;
	float m_RefreshRate;

	const float m_NearPlane = 0.1f;
	const float m_FarPlane = 10000;
	const float m_Fov = 50;

	bool m_IsSound;
	float m_SoundVolume;
};



