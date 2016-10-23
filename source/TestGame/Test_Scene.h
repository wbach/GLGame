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
		songo = make_shared<CPlayer>(&m_Game.GetInputManager(), CreatePositionVector(1, 1));
		songo->SetName("Player");
		unsigned int model_id = m_Loader.LoadMesh("Data/Meshes/Garen/garen_idle.fbx", true);
		unsigned int model_id2 = m_Loader.LoadMesh("Data/Meshes/Garen/garen_run.fbx", true);
		glm::vec3 normalized_size(0, 1.8f, 0);
		glm::mat4 nm = m_Loader.m_Models[model_id]->CalculateNormalizedMatrix(normalized_size.x, normalized_size.y, normalized_size.z);
		m_Loader.m_Models[model_id]->m_UseFakeLight = true;
		m_Loader.m_Models[model_id2]->m_UseFakeLight = true;
		songo->SetNormalizedMatrix(nm);
		songo->AddModel(model_id, "Data/Meshes/Garen/garen_idle.fbx");
		songo->AddModel(model_id2, "Data/Meshes/Garen/garen_idle.fbx");
		songo->SetNormalizedSize(normalized_size);
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
		
		normalized_size = glm::vec3(0, 1.f, 0);
		shared_ptr<CEntity> cube = CreateEntityFromFile("Data/Meshes/Cube.obj", false, CreatePositionVector(200 , 350 , 10));
		cube->SetName("Test physics cube");
		cube->m_IsSpecial = true;
		cube->SetNormalizedSize(normalized_size);
		cube->m_RigidBody = CRigidbody(SAabb(glm::vec3(-1, -1, -1) * 5.f, glm::vec3(1, 1, 1) * 5.f));
		AddEntity(cube, true);
		m_PhysicsEntities.push_back(cube);
		m_PhysicsScene.AddRigidBody(&cube->m_RigidBody);

		GLuint water_dudv	= m_Loader.LoadTexture("Data/Textures/waterDUDV.png");
		GLuint water_normal = m_Loader.LoadTexture("Data/Textures/waternormal.png");

		CWaterTile water(glm::vec3(100, 55, 110), 100, 0.3, water_dudv, water_normal, glm::vec4(43.0f/255.f, 106.f / 255.f, 134.f/255.f, 0.2));
		m_WaterTiles.push_back(water);	
		
		CGUIButton testButton(&m_Game.GetInputManager(), m_Loader.LoadTexture("Data/GUI/startGameButton.png"), m_Loader.LoadTexture("Data/GUI/hoverStartGameButton.png"), m_Loader.LoadTexture("Data/GUI/pushStartGamebutton.png"), "test", glm::vec2(-0.9, -0.95), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		m_Gui.guiButtons.push_back(testButton);

		m_MousePicker = CMousePicker(m_Camera, m_Game.GetDisplayManager().GetWindowSize(), m_Game.GetProjectionMatrix());

	

		//m_SceneParser.SaveToFile("Data/Maps/SavedTestMap.map", this);
		return 0;
	}
	virtual void PostInitialize() override
	{
		CTerrain* t1 = FindTerrainByName("Terrain0x-1");
		CTerrain* t2 = FindTerrainByName("Terrain0x0");
		if(t1 != nullptr && t2 !=nullptr)
		MergeTerrains(*t1, *t2, 1);

		t1 = FindTerrainByName("Terrain0x0");
		t2 = FindTerrainByName("Test_Terrain1x0");
		if (t1 != nullptr && t2 != nullptr)
			MergeTerrains(*t1, *t2, 0);

		t1 = FindTerrainByName("Terrain0x0");
		t2 = FindTerrainByName("Terrain0x1");
		if (t1 != nullptr && t2 != nullptr)
			MergeTerrains(*t1, *t2, 1);

		t1 = FindTerrainByName("Terrain0x1");
		t2 = FindTerrainByName("Terrain0x2");
		if (t1 != nullptr && t2 != nullptr)
			MergeTerrains(*t1, *t2, 1);

		return;
		shared_ptr<CEntity> grass = CreateEntityFromFile("Data/Terrain/Grass/grassGothic.obj", true, CreatePositionVector(100, 200));
		for (int y = 0; y < 100; y++)
		{
			for (int x = 1; x < 100; x++)
			{
				glm::vec2 pos = glm::vec2(100, 200) + 2.f*glm::vec2(rand() % 100, rand() % 100);
				grass->AddTransform(STransform(CreatePositionVector(pos)) );
				
			}
		}
		m_Loader.m_Models[grass->GetModelId()]->CreateTransformsVbo(grass->GetTransformMatrixes());
		grass->m_Instanced = true;
		grass->SetName("Test grass");
		grass->m_IsSpecial = true;
		m_Loader.m_Models[grass->GetModelId()]->m_UseFakeLight = true;
		AddEntity(grass, true);

		LoadTerrainsFloora("Data/Terrain/TestTerrainFlora.tf");
	}
	void setFirstCamera()
	{
		//songo->getReferencedPosition(), songo->getReferencedRotation()
        m_Camera.reset();
		m_Camera = make_shared<CFirstPersonCamera>(&m_Game.GetInputManager(), &m_Game.GetDisplayManager());
		m_Camera->SetPosition(CreatePositionVector(songo->GetPositionXZ(),10));
		m_Camera->SetPitch(5.6f);
		m_Camera->SetYaw(94.0f);
		m_Camera->SetProjectionMatrix(m_Game.GetProjectionMatrix());
	}
	void setThridCamera()
	{
		m_Camera.reset();
		m_Camera = make_shared<CThirdPersonCamera>(&m_Game.GetInputManager(),songo->GetReferencedPosition(), songo->GetReferencedRotation());
		m_Camera->SetProjectionMatrix(m_Game.GetProjectionMatrix());
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

			shared_ptr<CEntity> sphere = CreateEntityFromFile("Data/Meshes/Sphere/sphere.obj", false, CreatePositionVector(200 + static_cast<float>(rand() % 10 - 5)*5, 350 + static_cast<float>(rand()%10 - 5)*5, 100), glm::vec3(0), glm::vec3(5));
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
			if (m_CurrentTerrain != nullptr)
			{
				bool col;
				glm::vec3 point = m_MousePicker.GetMousePointOnTerrain(m_Game.GetInputManager().GetMousePosition(), *m_CurrentTerrain, col);
				
					for (CTerrain& terr : m_Terrains)
					{
						terr.PaintHeightMapPoint(glm::vec2(point.x, point.z),1);
					}
				

				/*m_CurrentTerrain->PaintHeightMap(point);
				float terrain_x = point.x - m_CurrentTerrain->m_Transform.position.x;
				float terrain_z = point.z - m_CurrentTerrain->m_Transform.position.z;
				float x = terrain_x / m_CurrentTerrain->GetSize();
				float z = terrain_z / m_CurrentTerrain->GetSize();*/
				//cout << x << "  " << z << endl;
			//	Utils::PrintVector("",point);
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
		float dt = static_cast<float>(m_Game.GetDisplayManager().GetDeltaTime());
		
		//sphere->IncrasePosition(m_Game.m_PhysicsScene.m_Rigibodys[sphere->m_PhysicsBodyIndex].m_AngularVelocity * dt);

		

		songo->calculateMove(dt);
		songo->move(dt, GetHeightOfTerrain(songo->GetPositionXZ()));
		
		glm::vec2 window_size = m_Game.GetDisplayManager().GetWindowSize();		

		if (m_Gui.guiButtons[0].CheckStatus(window_size) == GuiButtonState::ACTIVE)
		{
		//	m_Loader.ReloadTexture("Data/Terrain/TerrainTextures/blendMap.png", m_Terrains[0].m_BackgroundTexture[0]);
			
		}
		m_Loader.UpdateModels(static_cast<float>(m_Game.GetDisplayManager().GetDeltaTime()));

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

		for (CWaterTile& tile : m_WaterTiles)
		{
			tile.CleanUp();
		}

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



