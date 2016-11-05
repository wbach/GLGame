#pragma once
#include <string>
#include <vector>
#include "../BaseEngine/Engine/Scene.h"
#include "../BaseEngine/Camera/ThridPersonCamera.h"
#include "../BaseEngine/Camera/FirstPersonCamera.h"
#include "../BaseEngine/GUI/GUIButton.h"
#include "Player.h"
#include "../BaseEngine/Engine/GLgame.h"
class CTestSCene : public CScene
{
    CLight m_DirLight;
	CLight m_PointLight;
	shared_ptr<CEntity> sphere_plane;
    shared_ptr<CEntity> songo;	
	shared_ptr<CEntity>  lightsphere; 
	int m_CameraType = 0;
	float time_x = 0;	
	float camera_or_distance;
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
		

		for (unsigned int z = 0; z < m_TerrainsCount; z++)
			for (unsigned int x = 0; x < m_TerrainsCount; x++)
			{
				m_Terrains.push_back(CTerrain(m_Loader));
				CTerrain& current_terrain = m_Terrains.back();

				string name;
				name += "Terrain" + to_string(x) + "x" + to_string(z);

				//CreateNewEmptyTerrain(name, x, z);

				string height_map = "Data/Terrain/HeightMaps/terrain_" + name + ".terrain";// +name + ".terrain";

				string blend_map = "Data/Terrain/BlendMaps/terrain_" + name + ".png";
				//CreateEmptyHeightMap(height_map, 32, 32);
				current_terrain.Init(name,
					(float)x, (float)z,
					height_map, blend_map,
					"Data/Textures/G3_Nature_Ground_Grass_01_Diffuse_01.png", "Data/Textures/G3_Nature_Ground_Grass_01_Diffuse_01.png",
					"Data/Textures/165.png", "Data/Textures/165.png",
					"Data/Textures/grassFlowers.png", "Data/Textures/grassFlowers.png",
					"Data/Textures/G3_Nature_Wall_Stone_12_Diffuse_01.png", "Data/Textures/G3_Nature_Wall_Stone_12_Diffuse_01.png",
					"Data/Textures/G3_Architecture_Ground_City_03_Diffuse_01.png", "Data/Textures/G3_Architecture_Ground_City_03_Diffuse_01.png"
				);
	
				string grass_file = "Data/Terrain/GrassPositions/terrain_" + name + ".terrain";
				//current_terrain.GenerateGrassPositions(grass_file, 10000, 1.5f);
				current_terrain.InitGrassFromFile(grass_file, m_Loader.LoadTexture("Data/Textures/G3_Nature_Plant_Grass_06_Diffuse_01.png"));				
			
				//string sh_file = "Data/Terrain/Flora/shterrain_" + name + ".terrain";
				////current_terrain.GenerateGrassPositions(sh_file, 10000, 10.5f);
				//current_terrain.InitGrassFromFile(sh_file, m_Loader.LoadTexture("Data/Textures/shr15.png"));

				for (int x = 1; x <= 6; x++)
				{
					CAssimModel tree1(m_Loader.GetTextureLoader());
					string ftree_name = "Data/Meshes/woodland_pack_1/WOODLAND_PACK/WOODLAND_TREES/f_tree" + to_string(x) + "/obj__tree" + to_string(x) + ".obj";
					tree1.InitModel(ftree_name);
					tree1.m_UseFakeLight = true;
					current_terrain.m_Trees.push_back(tree1);
				}
				
				string trees_file = "Data/Terrain/TreesPositions/terrain_" + name + ".terrain";
				current_terrain.GenerateTreePositions(trees_file, 1000, 7.f, current_terrain.m_Trees.size(), 2);
				current_terrain.InitTreesFromFile(trees_file, glm::vec3(0, 10, 0));			


				//Flora
				CAssimModel fern(m_Loader.GetTextureLoader());
				string fernn = "Data/Meshes/Fern1/Fern.obj";
				fern.InitModel(fernn);
				fern.m_UseFakeLight = true;
				current_terrain.m_Trees.push_back(fern);

				string flora_file = "Data/Terrain/Flora/terrain_" + name + ".terrain";
			//	current_terrain.GenerateTreePositions(flora_file, 1000, 10.f, 1, 0.1, 6);
				current_terrain.InitTreesFromFile(flora_file, glm::vec3(0, 1, 0), 6);
			}		
		

		songo = make_shared<CPlayer>(&m_Game.GetInputManager(), CreatePositionVector(100, 120));
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
		songo->m_RigidBody = CRigidbody(SSphere(glm::vec3(0), 0.5));
		songo->m_RigidBody.m_Static = false;
		m_PhysicsScene.AddRigidBody(&songo->m_RigidBody);
		AddEntity(songo, true);
		m_PhysicsEntities.push_back(songo.get());

		if (m_CameraType == 0)
			setThridCamera();
		else
			setFirstCamera();
		camera_or_distance = m_Camera->GetDistance();
		//lightsphere = CreateEntityFromFile(Utils::ConvertToRelativePath("Data/Meshes/Sphere/sphere.obj"), ColiderType::NONE_COLIDER, false, CreatePositionVector(100.f, 120.f, 10.f));
		//AddEntity(lightsphere, true);


	//	sphere_plane = CreateEntityFromFile("Data/Meshes/housestorage1/houseStorage.obj", ColiderType::MESH_COLIDER ,false, CreatePositionVector(80.f, 140.f, 2.f), glm::vec3(0), glm::vec3(1.0));
	//	
	//	sphere_plane->SetName("sphere_plane");
	//	sphere_plane->m_IsSpecial = true;
	//	sphere_plane->SetNormalizedSize(glm::vec3(1000));
	//	model_id = sphere_plane->GetModelId();
	////	m_Loader.m_Models[model_id]->m_UseFakeLight = true;
	//	AddEntity(sphere_plane, true);

		//normalized_size = glm::vec3(0, 1.f, 0);
		//shared_ptr<CEntity> cube = CreateEntityFromFile("Data/Meshes/Cube.obj", false, CreatePositionVector(200 , 350 , 10));
		//cube->SetName("Test physics cube");
		//cube->m_IsSpecial = true;
		//cube->SetNormalizedSize(normalized_size);
		//cube->m_RigidBody = CRigidbody(SAabb(glm::vec3(-1, -1, -1) * 5.f, glm::vec3(1, 1, 1) * 5.f));
		//AddEntity(cube, true);
		//m_PhysicsEntities.push_back(cube);
		//m_PhysicsScene.AddRigidBody(&cube->m_RigidBody);

		GLuint water_dudv	= m_Loader.LoadTexture("Data/Textures/waterDUDV.png");
		GLuint water_normal = m_Loader.LoadTexture("Data/Textures/waternormal.png");

		CWaterTile water(glm::vec3(250, -0.1, 110), 100.f, 0.1f, water_dudv, water_normal, glm::vec4(43.0f/255.f, 106.f / 255.f, 134.f/255.f, 0.2));
		m_WaterTiles.push_back(water);	
		
		CGUIButton testButton(&m_Game.GetInputManager(), m_Loader.LoadTexture("Data/GUI/startGameButton.png"), m_Loader.LoadTexture("Data/GUI/hoverStartGameButton.png"), m_Loader.LoadTexture("Data/GUI/pushStartGamebutton.png"), "test", glm::vec2(-0.9, -0.95), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		m_Gui.guiButtons.push_back(testButton);

		m_MousePicker = CMousePicker(m_Camera, m_Game.GetDisplayManager().GetWindowSize(), m_Game.GetProjectionMatrix());
		m_MousePicker.SetTerrainsPtr(&m_Terrains, m_TerrainsCount);
		time_x = 0;

		m_DayNightCycle.SetTime(0.5f);
		return 0;
	}
	virtual void PostInitialize() override
	{		
	}
	void setFirstCamera()
	{
		//songo->getReferencedPosition(), songo->getReferencedRotation()
        m_Camera.reset();
		m_Camera = make_shared<CFirstPersonCamera>(&m_Game.GetInputManager(), &m_Game.GetDisplayManager());
	//	m_Camera->SetPosition(CreatePositionVector(songo->GetPositionXZ(),10));
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
		float dt = static_cast<float>(m_Game.GetDisplayManager().GetDeltaTime());

		m_GloabalTime += dt;
		m_DayNightCycle.Update(dt);
	//	m_GameTime.SetTime(1.f);

		//glm::vec3 light_point ;
		//CTerrain* terrain = m_MousePicker.GetMousePointOnTerrain(light_point, m_Game.GetInputManager().GetMousePosition());
		//light_point.y += 10;
		//for (CLight& l : m_Lights)
		//{
		//	l.SetPosition(light_point);
		//}
		//lightsphere->SetPosition(light_point);

		if (m_Game.GetInputManager().GetKey(KeyCodes::P))
		{
			for (CEntity* entity : m_PhysicsEntities)
			{
				entity->m_RigidBody.m_AngularVelocity = glm::vec3(0, 0, 0);
			}
		}
		if (m_Game.GetInputManager().GetKey(KeyCodes::T))
		{
			shared_ptr<CEntity> sphere = CreateEntityFromFile("Data/Meshes/Sphere/sphere.obj", ColiderType::SPHERE_COLIDER , glm::vec3(0), false, CreatePositionVector(200 + static_cast<float>(rand() % 10 - 5)*5, 350 + static_cast<float>(rand()%10 - 5)*5, 100), glm::vec3(0), glm::vec3(5));
			sphere->m_IsSpecial = true;
			sphere->m_RigidBody = CRigidbody(SSphere(glm::vec3(0), 5));
			g_pages_mutex.lock();
			AddEntity(sphere, true);
			m_PhysicsEntities.push_back(sphere.get());
			m_PhysicsScene.AddRigidBody(&sphere->m_RigidBody);
			g_pages_mutex.unlock();
		}
		
		if ( m_Game.GetDisplayManager().CheckActiveWindow() && !m_Game.GetInputManager().GetKey(KeyCodes::LCTRL) )
		{
			m_Camera->CalculateInput();
			if (m_CameraType == 0)
			m_Game.GetDisplayManager().ShowCoursor(false);
		}
		else
		{
			if (m_CameraType == 0)
			m_Game.GetDisplayManager().ShowCoursor(true);
		}
		m_Camera->Move();			

		if (m_Game.GetInputManager().GetKey(KeyCodes::M))
		{
			MergeAllTerrains();
		}

		if (m_Game.GetInputManager().GetKey(KeyCodes::Q))
		{
			glm::vec3 point;
			CTerrain* terr = m_MousePicker.GetMousePointOnTerrain(point, m_Game.GetInputManager().GetMousePosition());
			if (terr != nullptr)
			{	
				for (unsigned int y = (unsigned int)terr->GetPosition().z-1; y < (unsigned int)terr->GetPosition().z + 2; y++)
					for (unsigned int x = (unsigned int)terr->GetPosition().x - 1; x < (unsigned int)terr->GetPosition().x + 2; x++)
					{
						if (y < 0 || x < 0 || y > m_TerrainsCount || x > m_TerrainsCount)
							continue;
						CTerrain* terrain = GetTerrain(x, y);
						if (terrain == nullptr)
							continue;
						terrain->PaintHeightMapPoint(glm::vec2(point.x, point.z), m_BrushSize, m_HeightPaintStrength, m_ApplyLimits, m_HeightUpperLimit, m_HeightDownLimit);
					}
			}								
		}

			
		if (m_Game.GetInputManager().GetKey(KeyCodes::E))
		{
			glm::vec3 point;
			//CTerrain& terrain = m_Game.GetCurrentScene()->m_Terrains[x][y];
			CTerrain* terrain = m_MousePicker.GetMousePointOnTerrain(point, m_Game.GetInputManager().GetMousePosition());
			if (terrain != nullptr)
			{
				for (unsigned int y = (unsigned int)terrain->GetPosition().z - 1; y < (unsigned int)terrain->GetPosition().z + 2; y++)
					for (unsigned int x = (unsigned int)terrain->GetPosition().x - 1; x < (unsigned int)terrain->GetPosition().x + 2; x++)
					{
						if (y < 0 || x < 0 || y > m_TerrainsCount || x > m_TerrainsCount)
							continue;
						terrain->PaintBlendMap(point, m_BrushSize, m_PaintColor);
					}
			}			
		}		
		
		//sphere->IncrasePosition(m_Game.m_PhysicsScene.m_Rigibodys[sphere->m_PhysicsBodyIndex].m_AngularVelocity * dt);

		static_cast<CPlayer*>(songo.get())->calculateMove(dt);
		static_cast<CPlayer*>(songo.get())->move(dt, GetHeightOfTerrain(songo->GetPositionXZ()));
		

		//cout << "H: " << GetHeightOfWorld(songo->GetPositionXZ().x, songo->GetPositionXZ().y)<< endl;;

		//int xx, zz;
	//	TerrainNumber(songo->GetPositionXZ(), xx, zz);
		//if (xx >= 0 && zz >= 0)
		//Utils::PrintVector("Normal: ", );
	//	m_Terrains[xx][zz].GetNormalOfTerrain(songo->GetPositionXZ().x, songo->GetPositionXZ().y);
		glm::vec2 window_size = m_Game.GetDisplayManager().GetWindowSize();		

		if (m_Gui.guiButtons[0].CheckStatus(window_size) == GuiButtonState::ACTIVE)
		{
		//	m_Loader.ReloadTexture("Data/Terrain/TerrainTextures/blendMap.png", m_Terrains[0].m_BackgroundTexture[0]);
			
		}
		m_Loader.UpdateModels(static_cast<float>(dt));

		LockCameraUnderTerrain();

		return 0;
	}
	int CleanUp() override
	{
		m_Loader.CleanUp();
			
		for (CTerrain& t : m_Terrains)
			t.CleanUp();
		m_Terrains.clear();

		for (shared_ptr<CEntity>& entity : m_Entities)
		{
			entity->CleanUp();
			entity.reset();
		}			
		m_Entities.clear();


		for (CWaterTile& tile : m_WaterTiles)
		{
			tile.CleanUp();
		}
		shared_ptr<CCamera> m_Camera;
		vector<CWaterTile>	m_WaterTiles;
		vector<CLight>		m_Lights;
		
		m_WaterTiles.clear();
		m_DaySkyboxTextures.clear();
		m_NightSkyboxTextures.clear();
		m_Gui.guiTextures.clear();
		m_Gui.guiButtons.clear();
		m_Gui.guiTexts.clear();		
		m_Lights.clear();
		m_Camera.reset();
		songo.reset();
		m_PhysicsEntities.clear();
		m_PhysicsScene.m_Rigibodys.clear();
		return 0;
	}
	void LockCameraUnderTerrain()
	{
		float camera_r = 1.f;

		m_Camera->SetDistance(camera_or_distance);
		m_Camera->Move();
		glm::vec3 orginal_pos = m_Camera->GetPosition();
		glm::vec3 orginal_dir = m_Camera->GetDirection();
		CRigidbody crb(SSphere(m_Camera->GetPosition(), camera_r));

		float camera_distance = m_Camera->GetDistance();

		std::list<CRigidbody> bodys;
		float camera_min_distance = .25f;
		for (CEntity* entity : m_PhysicsEntities)
		{
			for (SFace& f : entity->m_RigidBody.m_Colider.GetFaces())
			{
				float x = Utils::IntersectTriangle(f.normal, orginal_pos, orginal_dir, f.vertexes);
				float y = Utils::IntersectTriangle(-f.normal, orginal_pos, orginal_dir, f.vertexes);
				if (x > 0 && x < camera_or_distance + 0.001f)
				{
					camera_distance -= x;
					if (camera_distance < camera_min_distance)
						camera_distance = camera_min_distance;
					m_Camera->SetDistance(camera_distance);
					m_Camera->Move();
				}
				if (y > 0 && y < camera_or_distance + 0.001f)
				{
					camera_distance -= y;
					if (camera_distance < camera_min_distance)
						camera_distance = camera_min_distance;
					m_Camera->SetDistance(camera_distance);
					m_Camera->Move();
				}

			}
			bodys.push_back(entity->m_RigidBody);
		}

		glm::vec2 camera_position = glm::vec2(m_Camera->GetPosition().x, m_Camera->GetPosition().z);
		int x, z;
		TerrainNumber(camera_position, x, z);
		if (x >= 0 && z >= 0)
		{
			CTerrain* terrain = GetTerrain(x, z);
			if (terrain == nullptr)
				return;
			float height = terrain->GetHeightofTerrain(camera_position.x, camera_position.y);
			if (m_Camera->GetPosition().y > height) return;
			m_Camera->SetPosition(glm::vec3(camera_position.x, height + .1f, camera_position.y));
			m_Camera->UpdateViewMatrix();
		}
		
		
	}
	const glm::mat4& GetViewMatrix() override { return m_Camera->GetViewMatrix(); }
};



