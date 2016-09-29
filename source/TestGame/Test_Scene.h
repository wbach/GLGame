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
	int Initialize() override{
		CGUIButton testButton(&m_Game.GetInputManager(), m_Loader.LoadTexture("Data/GUI/startGameButton.png"), m_Loader.LoadTexture("Data/GUI/hoverStartGameButton.png"), m_Loader.LoadTexture("Data/GUI/pushStartGamebutton.png"),"test", glm::vec2(-0.9,-0.95), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		m_Gui.guiButtons.push_back(testButton);

		cout << " Loading..." << endl;

        string terrainTexturePath = "Data/Terrain/TerrainTextures/";//TdkWN.png

		m_Terrains.push_back( CTerrain(m_Loader, terrainTexturePath + "TdkWN.png", 0, 0, m_Loader.LoadTexture(terrainTexturePath + "blendMap.png"),
			m_Loader.LoadTexture(terrainTexturePath + "grass.bmp"), m_Loader.LoadTexture(terrainTexturePath + "grassNormal.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "156.JPG"), m_Loader.LoadTexture(terrainTexturePath + "156.JPG"),
			m_Loader.LoadTexture(terrainTexturePath + "sand.jpg"), m_Loader.LoadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
			m_Loader.LoadTexture(terrainTexturePath + "177.JPG"), m_Loader.LoadTexture(terrainTexturePath + "177_norm.JPG")
			) );

		//terrains.push_back( CTerrain(loader, terrainTexturePath + "heightmap.png", -1, -1, loader.LoadTexture(terrainTexturePath + "blendMap.png"),
		//	loader.LoadTexture(terrainTexturePath + "grass.bmp"), loader.LoadTexture(terrainTexturePath + "grassNormal.jpg"),
		//	loader.LoadTexture(terrainTexturePath + "156.JPG"), loader.LoadTexture(terrainTexturePath + "156.JPG"),
		//	loader.LoadTexture(terrainTexturePath + "sand.jpg"), loader.LoadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
		//	loader.LoadTexture(terrainTexturePath + "177.JPG"), loader.LoadTexture(terrainTexturePath + "177_norm.JPG")
		//));

		//terrains.push_back(CTerrain(loader, terrainTexturePath + "heightmap.png", 0, -1, loader.LoadTexture(terrainTexturePath + "blendMap.png"),
		//	loader.LoadTexture(terrainTexturePath + "grass.bmp"), loader.LoadTexture(terrainTexturePath + "grassNormal.jpg"),
		//	loader.LoadTexture(terrainTexturePath + "156.JPG"), loader.LoadTexture(terrainTexturePath + "156.JPG"),
		//	loader.LoadTexture(terrainTexturePath + "sand.jpg"), loader.LoadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
		//	loader.LoadTexture(terrainTexturePath + "177.JPG"), loader.LoadTexture(terrainTexturePath + "177_norm.JPG")
		//));

		//terrains.push_back(CTerrain(loader, terrainTexturePath + "heightmap.png", -1, 0, loader.LoadTexture(terrainTexturePath + "blendMap.png"),
		//	loader.LoadTexture(terrainTexturePath + "grass.bmp"), loader.LoadTexture(terrainTexturePath + "grassNormal.jpg"),
		//	loader.LoadTexture(terrainTexturePath + "156.JPG"), loader.LoadTexture(terrainTexturePath + "156.JPG"),
		//	loader.LoadTexture(terrainTexturePath + "sand.jpg"), loader.LoadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
		//	loader.LoadTexture(terrainTexturePath + "177.JPG"), loader.LoadTexture(terrainTexturePath + "177_norm.JPG")
		//));
		
	/*	shared_ptr<CEntity> barrel;
		barrel = make_shared<CEntity>(createPositionVector(56, 47));
		barrel->model_id = loader.AssimpLoad("Data/Meshes/Barrel/barrel.obj");
		for (int x = -50; x < 50; x++) {
			barrel->addTransform(Transform(createPositionVector(56, 47 + 10 * x)));
		}
		terrains[0].addTerrainEntity(barrel);*/


		shared_ptr<CEntity> smallHause;
		smallHause = make_shared<CEntity>(CreatePositionVector(138,128,2.5), glm::vec3(0), glm::vec3(4));
		smallHause->m_ModelId = m_Loader.AssimpLoad("Data/Meshes/Gothic_smallHouse1/smallHouse1.obj");
		int tnr = TerrainNumber(smallHause->GetPositionXZ());
		if (tnr > 0)
			m_Terrains[tnr].AddTerrainEntity(smallHause);
		else
			AddEntity(smallHause);


		songo = make_shared<CPlayer>(&m_Game.GetInputManager(), CreatePositionVector(86, 47),glm::vec3(0),glm::vec3(2));
		songo->m_ModelId = m_Loader.AssimpLoad("Data/Meshes/Songo/songo2.obj");

		
	/*	m9 = make_shared<CEntity>(createPositionVector(86, 47,5), glm::vec3(-90,0,0), glm::vec3(0.25));
		m9->model_id = loader.AssimpLoad("Data/Meshes/M4A1/M4A1.obj");
		songo->addSubbEntity(m9);*/
		AddEntity(songo);
	

		setThridCamera();
		//setFirstCamera();
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
		m_Camera = make_shared<CFirstPersonCamera>();
		m_Camera->SetPosition(CreatePositionVector(songo->GetPositionXZ(),10));
		m_Camera->SetPitch(5.6f);
		m_Camera->SetYaw(94.0f);
	}
	void setThridCamera(){
		m_Camera.reset();
		m_Camera = make_shared<CThirdPersonCamera>(&m_Game.GetInputManager(),songo->GetReferencedPosition(), songo->GetReferencedRotation());
	}
	int Update() override{

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

//		m9->setPosition(songo->getPosition() + glm::vec3(0, 7.5, 0));
//		m9->setRotation(songo->getRotation()+ glm::vec3(camera->getPitch(),0,0));

	/*	switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_F1: cout << "Camera position : " << Utils::ToString(m_Camera->GetPosition()); break;
			case SDLK_F2: cout << "Camera angles : " << m_Camera->GetPitch() << " " << m_Camera->GetYaw() << " " << m_Camera->GetRoll() << endl; break;
            case SDLK_F3: thridCamera = !thridCamera; thridCamera ? setThridCamera() : setFirstCamera(); break;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y == 1)
			{
				m_Camera->CalculateZoom(-1);
			}
			else if (event.wheel.y == -1)
			{
				m_Camera->CalculateZoom(1);
			}break;
		
		break;
		}
		event = SDL_Event(); */
		return 0;
	}
	int CleanUp() override{
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
	const glm::mat4& GetViewMatrix() override { return m_Camera->GetViewMatrix(); }
};



