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
#include "../Loader/XML/XMLSceneParser.h"

using namespace std;

class CGame 
{
public:
	CGame();
	bool m_FroceQuit = false;

	void Initialize(std::shared_ptr<CApi> api);
	void GameLoop();
	void PhysicsLoop();
	void Uninitialize();

	void SetWindowSize(glm::vec2 size);

	CInputManager& GetInputManager() { return m_InputManager; }
	CDisplayManager& GetDisplayManager() { return m_DisplayManager; }
	CMasterRenderer& GetMasterRenderer() { return m_MasterRenderer; }

	void CreateProjectionMatrix();
	const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }

    int SetCurrentScene(int i);
    void LoadScene();
	void InitializeScene() ;
	void AddScene(shared_ptr<CScene> scene);

	void RenderStartSeries();
	float Fade(float delta_time);
	float FadeIn(float delta_time, float start_time, float durration);
	float FadeOut(float delta_time, float start_time, float durration);

	int ReadConfiguration(string file_name);

	void(*OnGameLoopRun)();

	//For editor
	shared_ptr<CScene>& GetCurrentScene();	

	CXmlSceneParser m_SceneParser;
	
	const float& GetShadowMapSize();
	const bool& IsShadows();

	//Loading in this thread
	bool m_IsLoadedFromFile = false;
	std::string m_SceneFileToLoad;
	
	const glm::vec2& GetMaxTextureResolution();

private:
	CDisplayManager m_DisplayManager;
	CInputManager	m_InputManager;

	LoadingShader m_LoadingShader;

	vector<shared_ptr<CScene>> m_Scenes;
	shared_ptr<CScene> m_CurrScene;

	CGUIRenderer	m_GuiRenderer;
	CMasterRenderer m_MasterRenderer;

	glm::vec3 m_BackgroundColour;
	glm::mat4 m_ProjectionMatrix;

	float m_ViewDistance;
	float m_GrassViewDistance;

	bool	m_IsShadows;
	float	m_ShadowMapSize;

	float m_WaterQuality;
	glm::vec2 m_MaxTextureResolution;

	bool m_IsLoading;
	bool m_IsFullScreen;	

	glm::vec2	m_WindowSize;
	string		m_WindowName;
	int		m_RefreshRate;

	const float m_NearPlane = 0.1f;
	const float m_FarPlane = 500;
	const float m_Fov = 70;

	bool	m_IsSound;
	float	m_SoundVolume;

	int m_ApiMessage;
};



