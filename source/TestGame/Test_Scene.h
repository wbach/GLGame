#pragma once
#include <string>
#include <vector>
#include "../BaseEngine/Loader/XML/XMLSceneParser.h"
#include "../BaseEngine/Engine/Scene.h"
#include "../BaseEngine/Camera/ThridPersonCamera.h"
#include "../BaseEngine/Camera/FirstPersonCamera.h"
#include "../BaseEngine/GUI/GUIButton.h"
#include "Player.h"

class CTestSCene : public CScene
{
    CLight m_DirLight;
	CLight m_PointLight;

    shared_ptr<CPlayer> songo;	
	shared_ptr<CEntity> m9;

	int m_CameraType = 0;

	CXmlSceneParser m_SceneParser;
public:
    bool thridCamera = true;
	CTestSCene(CGame& game, int camera_type)
		: CScene(game)
		, m_CameraType(camera_type)
	{
		m_Name = "Test Scene";
	}
	int Initialize() override
	{
		songo = make_shared<CPlayer>(&m_Game.GetInputManager(), CreatePositionVector(86, 47), glm::vec3(0), glm::vec3(0.05));
		songo->SetName("Player");
		songo->AddModel(m_Loader.LoadMesh("Data/Meshes/Garen/garen_idle.fbx", true), "Data/Meshes/Garen/garen_idle.fbx");
		songo->AddModel(m_Loader.LoadMesh("Data/Meshes/Garen/garen_run.fbx", true), "Data/Meshes/Garen/garen_idle.fbx");
		AddEntity(songo, true);


		if (m_CameraType == 0)
			setThridCamera();
		else
			setFirstCamera();
		m_SceneParser.LoadScene("Data/Maps/TestMap.map", this);
		

		vector<string> DAY_TEXTURES_FILES;
		vector<string> NIGHT_TEXTURES_FILES;

		DAY_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/right.bmp");
		DAY_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/left.bmp");
		DAY_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/top.bmp");
		DAY_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/bottom.bmp");
		DAY_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/back.bmp");
		DAY_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/front.bmp");
		NIGHT_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/right.bmp");
		NIGHT_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/left.bmp");
		NIGHT_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/top.bmp");
		NIGHT_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/bottom.bmp");
		NIGHT_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/back.bmp");
		NIGHT_TEXTURES_FILES.push_back("Data/Skybox/TropicalSunnyDay/front.bmp");

		GLuint day_texture = m_Loader.LoadCubeMap(DAY_TEXTURES_FILES);
		GLuint night_texture = m_Loader.LoadCubeMap(DAY_TEXTURES_FILES);

		m_Game.GetMasterRenderer().SetSkyBoxTextures(day_texture, night_texture);
		int skybox_cube = m_Loader.LoadMesh("Data/Meshes/SkyBox/cube.obj");
		m_Game.GetMasterRenderer().SetSkyBoxMeshId(m_Loader.m_Models[skybox_cube]->GetMeshes()[0].GetVao(), m_Loader.m_Models[skybox_cube]->GetMeshes()[0].GetVertexCount());


		CGUIButton testButton(&m_Game.GetInputManager(), m_Loader.LoadTexture("Data/GUI/startGameButton.png"), m_Loader.LoadTexture("Data/GUI/hoverStartGameButton.png"), m_Loader.LoadTexture("Data/GUI/pushStartGamebutton.png"), "test", glm::vec2(-0.9, -0.95), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		m_Gui.guiButtons.push_back(testButton);

		shared_ptr<CEntity>  entity = CreateEntityFromFile("Data/Meshes/Gothic_smallHouse1/smallHouse1.obj", CreatePositionVector(138, 128, 2.5), glm::vec3(0), glm::vec3(4));
		//AddSubEntity(entity)
		AddEntity(entity, true);
		shared_ptr<CEntity>  barrel = CreateEntityFromFile("Data/Meshes/Barrel/barrel.obj", CreatePositionVector(86, 128, 0));
		AddSubEntity(entity, barrel);
		shared_ptr<CEntity>  barrel2 = CreateEntityFromFile("Data/Meshes/Barrel/barrel.obj", CreatePositionVector(100, 128, 0));
		AddSubEntity(barrel, barrel2);
		shared_ptr<CEntity>  barrel3 = CreateEntityFromFile("Data/Meshes/Barrel/barrel.obj", CreatePositionVector(120, 128, 0));
		AddSubEntity(barrel2, barrel3);

		
		

		
	//	camera->attachToObject();

		m_DirLight = CLight(glm::vec3(0.5));
		m_Lights.push_back(m_DirLight);

		glm::vec3 point_ligt_position = CreatePositionVector(86, 47);
		point_ligt_position.y += 10.f;
		m_PointLight = CLight(point_ligt_position, glm::vec3(0.5f, .5f, 0.f), glm::vec3(0, 0.1, 0));
		m_Lights.push_back(m_PointLight);		

		m_SceneParser.SaveToFile("Data/Maps/SavedTestMap.map", this);
		return 0;
	}
	void setFirstCamera(){
		//songo->getReferencedPosition(), songo->getReferencedRotation()
        m_Camera.reset();
		m_Camera = make_shared<CFirstPersonCamera>(&m_Game.GetInputManager(), &m_Game.GetDisplayManager());
		m_Camera->SetPosition(CreatePositionVector(songo->GetPositionXZ(),10));
		m_Camera->SetPitch(5.6f);
		m_Camera->SetYaw(94.0f);
	}
	void setThridCamera(){
		m_Camera.reset();
		m_Camera = make_shared<CThirdPersonCamera>(&m_Game.GetInputManager(),songo->GetReferencedPosition(), songo->GetReferencedRotation());
	}
	int Update() override
	{
		if ( m_Game.GetDisplayManager().CheckActiveWindow() && !m_Game.GetInputManager().GetKey(KeyCodes::LCTRL) )
		{
			m_Camera->CalculateInput();
			//m_Game.GetDisplayManager().ShowCoursor(false);
		}
		else
		{
			//m_Game.GetDisplayManager().ShowCoursor(true);
		}
		m_Camera->Move();
		

		float dt = m_Game.GetDisplayManager().GetDeltaTime();
		songo->calculateMove(dt);
		songo->move(dt, GetHeightOfTerrain(songo->GetPositionXZ()));

		glm::vec2 window_size = m_Game.GetDisplayManager().GetWindowSize();		

		if (m_Gui.guiButtons[0].CheckStatus(window_size) == GuiButtonState::ACTIVE)
			return 2;

		m_Loader.UpdateModels(m_Game.GetDisplayManager().GetDeltaTime());

		LockCameraUnderTerrain();

		return 0;
	}
	int CleanUp() override
	{
		m_Loader.CleanUp();
		for (unsigned int x = 0; x < m_Terrains.size(); x++) {
			m_Terrains[x].CleanUp();
		}
		m_Terrains.clear();
		m_Entities.clear();
		m_Gui.guiButtons.clear();
		m_Gui.guiTexts.clear();
		m_Gui.guiTextures.clear();
		m_Lights.clear();
		m_Camera.reset();
		return 0;
	}
	void LockCameraUnderTerrain()
	{
		glm::vec2 camera_position = glm::vec2(m_Camera->GetPosition().x, m_Camera->GetPosition().z);
		int tnr = TerrainNumber(camera_position);
		if (tnr >= 0)
		{
			float height = m_Terrains[tnr].GetHeightofTerrain(camera_position.x, camera_position.y);
			if (m_Camera->GetPosition().y > height) return;
			m_Camera->SetPosition(glm::vec3(camera_position.x, height + .1f, camera_position.y));
			m_Camera->UpdateViewMatrix();
		}
	}
	const glm::mat4& GetViewMatrix() override { return m_Camera->GetViewMatrix(); }
};



