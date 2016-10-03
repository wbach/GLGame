#pragma once
#include <string>
#include <vector>
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
public:
    bool thridCamera = true;
	CTestSCene(CGame& game)
		: CScene(game)
	{
		m_Name = "Test Scene";
	}
	int Initialize() override
	{
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

		cout << " Loading..." << endl;

		string terrainTexturePath = "Data/Terrain/TerrainTextures/";//TdkWN.png

		m_Terrains.push_back(CTerrain(m_Loader, terrainTexturePath + "TdkWN.png", 0, 0, m_Loader.LoadTexture(terrainTexturePath + "blendMap.png"),
			m_Loader.LoadTexture(terrainTexturePath + "grass.bmp"), m_Loader.LoadTexture(terrainTexturePath + "grassNormal.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "156.JPG"), m_Loader.LoadTexture(terrainTexturePath + "156.JPG"),
			m_Loader.LoadTexture(terrainTexturePath + "sand.jpg"), m_Loader.LoadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "177.JPG"), m_Loader.LoadTexture(terrainTexturePath + "177_norm.JPG")
		));

		m_Terrains.push_back( CTerrain(m_Loader, terrainTexturePath + "heightmap.png", -1, -1, m_Loader.LoadTexture(terrainTexturePath + "blendMap.png"),
			m_Loader.LoadTexture(terrainTexturePath + "grass.bmp"), m_Loader.LoadTexture(terrainTexturePath + "grassNormal.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "156.JPG"), m_Loader.LoadTexture(terrainTexturePath + "156.JPG"),
			m_Loader.LoadTexture(terrainTexturePath + "sand.jpg"), m_Loader.LoadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "177.JPG"), m_Loader.LoadTexture(terrainTexturePath + "177_norm.JPG")
		));

		m_Terrains.push_back(CTerrain(m_Loader, terrainTexturePath + "heightmap.png", 0, -1, m_Loader.LoadTexture(terrainTexturePath + "blendMap.png"),
			m_Loader.LoadTexture(terrainTexturePath + "grass.bmp"), m_Loader.LoadTexture(terrainTexturePath + "grassNormal.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "156.JPG"), m_Loader.LoadTexture(terrainTexturePath + "156.JPG"),
			m_Loader.LoadTexture(terrainTexturePath + "sand.jpg"), m_Loader.LoadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "177.JPG"), m_Loader.LoadTexture(terrainTexturePath + "177_norm.JPG")
		));

		m_Terrains.push_back(CTerrain(m_Loader, terrainTexturePath + "heightmap.png", -1, 0, m_Loader.LoadTexture(terrainTexturePath + "blendMap.png"),
			m_Loader.LoadTexture(terrainTexturePath + "grass.bmp"), m_Loader.LoadTexture(terrainTexturePath + "grassNormal.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "156.JPG"), m_Loader.LoadTexture(terrainTexturePath + "156.JPG"),
			m_Loader.LoadTexture(terrainTexturePath + "sand.jpg"), m_Loader.LoadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "177.JPG"), m_Loader.LoadTexture(terrainTexturePath + "177_norm.JPG")
		));

	/*	shared_ptr<CEntity> barrel;
		barrel = make_shared<CEntity>(createPositionVector(56, 47));
		barrel->model_id = loader.AssimpLoad("Data/Meshes/Barrel/barrel.obj");
		for (int x = -50; x < 50; x++) {
			barrel->addTransform(Transform(createPositionVector(56, 47 + 10 * x)));
		}
		terrains[0].addTerrainEntity(barrel);*/


		shared_ptr<CEntity> smallHause;
		smallHause = make_shared<CEntity>(CreatePositionVector(138, 128, 2.5), glm::vec3(0), glm::vec3(4));
		smallHause->AddModel(m_Loader.LoadMesh("Data/Meshes/Gothic_smallHouse1/smallHouse1.obj"));
		int tnr = TerrainNumber(smallHause->GetPositionXZ());
		if (tnr > 0)
			m_Terrains[tnr].AddTerrainEntity(smallHause);
		else
			AddEntity(smallHause);


		shared_ptr<CEntity> barrel;
		barrel = make_shared<CEntity>(CreatePositionVector(86, 128), glm::vec3(0), glm::vec3(1));
		barrel->AddModel(m_Loader.LoadMesh("Data/Meshes/Barrel/barrel.obj"));
		tnr = TerrainNumber(barrel->GetPositionXZ());
		if (tnr > 0)
			m_Terrains[tnr].AddTerrainEntity(barrel);
		else
			AddEntity(barrel);


		/*shared_ptr<CEntity> fbx_test;
		fbx_test = make_shared<CEntity>(CreatePositionVector(86, 70), glm::vec3(0), glm::vec3(0.05));
		fbx_test->AddModel(m_Loader.LoadMesh("Data/Meshes/Garen/garen_run.fbx", true));
		tnr = TerrainNumber(fbx_test->GetPositionXZ());
		if (tnr > 0)
		{
			m_Terrains[tnr].AddTerrainEntity(fbx_test);
		}
		else
		{		
			AddEntity(fbx_test);
			
		}*/		

		songo = make_shared<CPlayer>(&m_Game.GetInputManager(), CreatePositionVector(86, 47),glm::vec3(0),glm::vec3(0.05));
		songo->AddModel(m_Loader.LoadMesh("Data/Meshes/Garen/garen_idle.fbx", true));
		songo->AddModel(m_Loader.LoadMesh("Data/Meshes/Garen/garen_run.fbx", true));
		
	/*	m9 = make_shared<CEntity>(createPositionVector(86, 47,5), glm::vec3(-90,0,0), glm::vec3(0.25));
		m9->model_id = loader.AssimpLoad("Data/Meshes/M4A1/M4A1.obj");
		songo->addSubbEntity(m9);*/
		AddEntity(songo);
	

		//setThridCamera();
		setFirstCamera();
	//	camera->attachToObject();

		m_DirLight = CLight(glm::vec3(0.5));
		m_Lights.push_back(m_DirLight);

		glm::vec3 point_ligt_position = CreatePositionVector(86, 47);
		point_ligt_position.y += 10.f;
		m_PointLight = CLight(point_ligt_position, glm::vec3(0.5f, .5f, 0.f), glm::vec3(0, 0.1, 0));
		m_Lights.push_back(m_PointLight);


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
			m_Game.GetDisplayManager().ShowCoursor(false);
		}
		else
		{
			m_Game.GetDisplayManager().ShowCoursor(true);
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



