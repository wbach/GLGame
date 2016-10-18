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
	int m_CameraType = 0;

	
	
public:
    bool thridCamera = true;
	CTestSCene(CGame& game, int camera_type)
		: CScene(game)
		, m_CameraType(camera_type)
	{
		m_Name = "Test Scene";
		m_SceneFile = "Data/Maps/TestMap.map";
	}
	int Initialize() override
	{
		songo = make_shared<CPlayer>(&m_Game.GetInputManager(), CreatePositionVector(100, 100));
		songo->SetName("Player");
		unsigned int model_id = m_Loader.LoadMesh("Data/Meshes/Garen/garen_idle.fbx", true);
		unsigned int model_id2 = m_Loader.LoadMesh("Data/Meshes/Garen/garen_run.fbx", true);
		glm::mat4 nm = m_Loader.m_Models[model_id]->CalculateNormalizedMatrix(0, 1.8f, 0);
		songo->SetNormalizedMatrix(nm);
		songo->AddModel(model_id, "Data/Meshes/Garen/garen_idle.fbx");
		songo->AddModel(model_id2, "Data/Meshes/Garen/garen_idle.fbx");
		//m_IsSpecial - enity created in code not from file
		songo->m_IsSpecial = true;
		songo->m_RigidBody = CRigidbody(SSphere(glm::vec3(0), 5));
		m_PhysicsScene.AddRigidBody(&songo->m_RigidBody);
		AddEntity(songo, true);
		m_PhysicsEntities.push_back(songo);

		if (m_CameraType == 0)
			setThridCamera();
		else
			setFirstCamera();
		

		shared_ptr<CEntity> cube = CreateEntityFromFile("Data/Meshes/Cube.obj", false, CreatePositionVector(200 , 350 , 10));
		cube->SetName("Test physics cube");
		cube->m_IsSpecial = true;
		cube->m_RigidBody = CRigidbody(SAabb(glm::vec3(-1, -1, -1) * 5.f, glm::vec3(1, 1, 1) * 5.f));
		AddEntity(cube, true);
		m_PhysicsEntities.push_back(cube);
		m_PhysicsScene.AddRigidBody(&cube->m_RigidBody);

		

	
		
		CGUIButton testButton(&m_Game.GetInputManager(), m_Loader.LoadTexture("Data/GUI/startGameButton.png"), m_Loader.LoadTexture("Data/GUI/hoverStartGameButton.png"), m_Loader.LoadTexture("Data/GUI/pushStartGamebutton.png"), "test", glm::vec2(-0.9, -0.95), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		m_Gui.guiButtons.push_back(testButton);

		m_MousePicker = CMousePicker(m_Camera, m_Game.GetDisplayManager().GetWindowSize(), m_Game.GetProjectionMatrix());
		//m_SceneParser.SaveToFile("Data/Maps/SavedTestMap.map", this);
		return 0;
	}
	virtual void PostInitialize() override
	{
		shared_ptr<CEntity> grass = CreateEntityFromFile("Data/Terrain/Grass/grassGothic.obj", true, CreatePositionVector(200, 100));
		for (int y = 0; y < 100; y++)
		{
			for (int x = 1; x < 100; x++)
			{
				grass->AddTransform(STransform(CreatePositionVector(200 + (rand()%2 +1)* x, 100 + (rand() % 2 + 1) * y)));
				
			}
		}
		m_Loader.m_Models[grass->GetModelId()]->CreateTransformsVbo(grass->GetTransformMatrixes());
		
		grass->SetName("Test grass");
		grass->m_IsSpecial = true;
		AddEntity(grass, true);
	}
	void setFirstCamera()
	{
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
		if (m_Game.GetInputManager().GetKey(KeyCodes::P))
		{
			for (shared_ptr<CEntity> entity : m_PhysicsEntities)
			{
				entity->m_RigidBody.m_AngularVelocity = glm::vec3(0, 0, 0);
			}
		}
		if (m_Game.GetInputManager().GetKey(KeyCodes::T))
		{

			shared_ptr<CEntity> sphere = CreateEntityFromFile("Data/Meshes/Sphere/sphere.obj", false, CreatePositionVector(200 + (rand() % 10 - 5)*5, 350 + (rand()%10 - 5)*5, 100), glm::vec3(0), glm::vec3(5));
			sphere->m_IsSpecial = true;
			sphere->m_RigidBody = CRigidbody(SSphere(glm::vec3(0), 5));
			g_pages_mutex.lock();
			AddEntity(sphere, true);
			m_PhysicsEntities.push_back(sphere);
			m_PhysicsScene.AddRigidBody(&sphere->m_RigidBody);
			g_pages_mutex.unlock();

		}
		
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
		if (m_Game.GetInputManager().GetKey(KeyCodes::Q))
		{
			for (CTerrain& terrain : m_Terrains)
			{
				bool is_col;
				glm::vec3 point = m_MousePicker.GetMousePointOnTerrain(m_Game.GetInputManager().GetMousePosition(), terrain, is_col);
				terrain.PaintHeightMap(point);
				if (is_col) break;
			}
		}
		if (m_Game.GetInputManager().GetKey(KeyCodes::E))
		{
			for (CTerrain& terrain : m_Terrains)
			{
				bool is_col;
				glm::vec3 point = m_MousePicker.GetMousePointOnTerrain(m_Game.GetInputManager().GetMousePosition(), terrain, is_col);
				terrain.PaintBlendMap(point);
				if (is_col) break;
			}
		}
		float dt = m_Game.GetDisplayManager().GetDeltaTime();
		
		//sphere->IncrasePosition(m_Game.m_PhysicsScene.m_Rigibodys[sphere->m_PhysicsBodyIndex].m_AngularVelocity * dt);

		

		songo->calculateMove(dt);
		songo->move(dt, GetHeightOfTerrain(songo->GetPositionXZ()));
		
		glm::vec2 window_size = m_Game.GetDisplayManager().GetWindowSize();		

		if (m_Gui.guiButtons[0].CheckStatus(window_size) == GuiButtonState::ACTIVE)
		{
		//	m_Loader.ReloadTexture("Data/Terrain/TerrainTextures/blendMap.png", m_Terrains[0].m_BackgroundTexture[0]);
			
		}
		m_Loader.UpdateModels(m_Game.GetDisplayManager().GetDeltaTime());

		LockCameraUnderTerrain();

		return 0;
	}
	int CleanUp() override
	{
		m_Loader.CleanUp();
		for (unsigned int x = 0; x < m_Terrains.size(); x++) 
		{
			m_Terrains[x].CleanUp();
		}
		m_Terrains.clear();
		for (shared_ptr<CEntity>& entity : m_Entities)
			entity->CleanUp();
		m_Entities.clear();

		m_Gui.guiButtons.clear();
		m_Gui.guiTexts.clear();
		m_Gui.guiTextures.clear();
		m_Lights.clear();
		m_Camera.reset();
		songo.reset();
		m_PhysicsEntities.clear();
		m_PhysicsScene.m_Rigibodys.clear();
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



