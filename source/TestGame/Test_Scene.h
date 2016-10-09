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
		m_SceneFile = "Data/Maps/SavedTestMap.map";
	}
	int Initialize() override
	{
		songo = make_shared<CPlayer>(&m_Game.GetInputManager(), CreatePositionVector(86, 47), glm::vec3(0), glm::vec3(0.05));
		songo->SetName("Player");
		songo->AddModel(m_Loader.LoadMesh("Data/Meshes/Garen/garen_idle.fbx", true), "Data/Meshes/Garen/garen_idle.fbx");
		songo->AddModel(m_Loader.LoadMesh("Data/Meshes/Garen/garen_run.fbx", true), "Data/Meshes/Garen/garen_idle.fbx");
		//m_IsSpecial - enity created in code not from file
		songo->m_IsSpecial = true;
		AddEntity(songo, true);


		if (m_CameraType == 0)
			setThridCamera();
		else
			setFirstCamera();

			

		

		CGUIButton testButton(&m_Game.GetInputManager(), m_Loader.LoadTexture("Data/GUI/startGameButton.png"), m_Loader.LoadTexture("Data/GUI/hoverStartGameButton.png"), m_Loader.LoadTexture("Data/GUI/pushStartGamebutton.png"), "test", glm::vec2(-0.9, -0.95), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		m_Gui.guiButtons.push_back(testButton);

		m_MousePicker = CMousePicker(m_Camera, m_Game.GetDisplayManager().GetWindowSize(), m_Game.GetProjectionMatrix());
		//m_SceneParser.SaveToFile("Data/Maps/SavedTestMap.map", this);
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
		if (m_Game.GetInputManager().GetKey(KeyCodes::Q))
		{
			for (CTerrain& terrain : m_Terrains)
			{
				glm::vec3 point = m_MousePicker.GetMousePointOnTerrain(m_Game.GetInputManager().GetMousePosition(), terrain);
				//Utils::PrintVector("Mouse terrain pos: ", point);
				//terrain.PaintBlendMap(point);
				terrain.PaintHeightMap(point);
			}
		}

		float dt = m_Game.GetDisplayManager().GetDeltaTime();
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



