#ifndef TEST_SCENE_H
#define TEST_SCENE_H
#pragma once
#include <string>
#include <vector>
#include "../BaseEngine/Engine/Scene.h"
#include "../BaseEngine/Camera/ThridPersonCamera.h"
#include "../BaseEngine/Camera/FirstPersonCamera.h"
#include "../BaseEngine/GUI/GUIButton.h"
#include "Player.h"

class CTestSCene : public CScene {
    CLight dirLight;
    shared_ptr<CPlayer> songo;	
	shared_ptr<CEntity> m9;
//	CGUITexture testTexture;
public:
    bool thridCamera = true;
	CTestSCene(){
		name = "Test Scene";
	}
	int initialize() override{
		CGUIButton testButton(loader.loadTexture("Data/GUI/startGameButton.png"), loader.loadTexture("Data/GUI/hoverStartGameButton.png"), loader.loadTexture("Data/GUI/pushStartGamebutton.png"),"test", glm::vec2(-0.9,-0.95), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		gui.guiButtons.push_back(testButton);

		cout << " Loading..." << endl;

        string terrainTexturePath = "Data/Terrain/TerrainTextures/";//TdkWN.png

		terrains.push_back( CTerrain(loader, terrainTexturePath + "TdkWN.png", 0, 0, loader.loadTexture(terrainTexturePath + "blendMap.png"),
			loader.loadTexture(terrainTexturePath + "grass.bmp"), loader.loadTexture(terrainTexturePath + "grassNormal.jpg"),
			loader.loadTexture(terrainTexturePath + "156.JPG"), loader.loadTexture(terrainTexturePath + "156.JPG"),
			loader.loadTexture(terrainTexturePath + "sand.jpg"), loader.loadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
			loader.loadTexture(terrainTexturePath + "177.JPG"), loader.loadTexture(terrainTexturePath + "177_norm.JPG")
			) );

		//terrains.push_back( CTerrain(loader, terrainTexturePath + "heightmap.png", -1, -1, loader.loadTexture(terrainTexturePath + "blendMap.png"),
		//	loader.loadTexture(terrainTexturePath + "grass.bmp"), loader.loadTexture(terrainTexturePath + "grassNormal.jpg"),
		//	loader.loadTexture(terrainTexturePath + "156.JPG"), loader.loadTexture(terrainTexturePath + "156.JPG"),
		//	loader.loadTexture(terrainTexturePath + "sand.jpg"), loader.loadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
		//	loader.loadTexture(terrainTexturePath + "177.JPG"), loader.loadTexture(terrainTexturePath + "177_norm.JPG")
		//));

		//terrains.push_back(CTerrain(loader, terrainTexturePath + "heightmap.png", 0, -1, loader.loadTexture(terrainTexturePath + "blendMap.png"),
		//	loader.loadTexture(terrainTexturePath + "grass.bmp"), loader.loadTexture(terrainTexturePath + "grassNormal.jpg"),
		//	loader.loadTexture(terrainTexturePath + "156.JPG"), loader.loadTexture(terrainTexturePath + "156.JPG"),
		//	loader.loadTexture(terrainTexturePath + "sand.jpg"), loader.loadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
		//	loader.loadTexture(terrainTexturePath + "177.JPG"), loader.loadTexture(terrainTexturePath + "177_norm.JPG")
		//));

		//terrains.push_back(CTerrain(loader, terrainTexturePath + "heightmap.png", -1, 0, loader.loadTexture(terrainTexturePath + "blendMap.png"),
		//	loader.loadTexture(terrainTexturePath + "grass.bmp"), loader.loadTexture(terrainTexturePath + "grassNormal.jpg"),
		//	loader.loadTexture(terrainTexturePath + "156.JPG"), loader.loadTexture(terrainTexturePath + "156.JPG"),
		//	loader.loadTexture(terrainTexturePath + "sand.jpg"), loader.loadTexture(terrainTexturePath + "white-sands-sand_NORM.jpg"),
		//	loader.loadTexture(terrainTexturePath + "177.JPG"), loader.loadTexture(terrainTexturePath + "177_norm.JPG")
		//));
		
	/*	shared_ptr<CEntity> barrel;
		barrel = make_shared<CEntity>(createPositionVector(56, 47));
		barrel->model_id = loader.assimpLoad("Data/Meshes/Barrel/barrel.obj");
		for (int x = -50; x < 50; x++) {
			barrel->addTransform(Transform(createPositionVector(56, 47 + 10 * x)));
		}
		terrains[0].addTerrainEntity(barrel);*/


		shared_ptr<CEntity> smallHause;
		smallHause = make_shared<CEntity>(createPositionVector(138,128,2.5), glm::vec3(0), glm::vec3(4));
		smallHause->model_id = loader.assimpLoad("Data/Meshes/Gothic_smallHouse1/smallHouse1.obj");
		int tnr = terrainNumber(smallHause->getPositionXZ());
		if (tnr > 0)
			terrains[tnr].addTerrainEntity(smallHause);
		else
			addEntity(smallHause);


		songo = make_shared<CPlayer>(createPositionVector(86, 47),glm::vec3(0),glm::vec3(2));
		songo->model_id = loader.assimpLoad("Data/Meshes/Songo/songo2.obj");

		
	/*	m9 = make_shared<CEntity>(createPositionVector(86, 47,5), glm::vec3(-90,0,0), glm::vec3(0.25));
		m9->model_id = loader.assimpLoad("Data/Meshes/M4A1/M4A1.obj");
		songo->addSubbEntity(m9);*/
		addEntity(songo);
	

		setThridCamera();
		//setFirstCamera();
	//	camera->attachToObject();

		dirLight = CLight(glm::vec3(5000),glm::vec3(1.0));
		lights.push_back(dirLight);
		return 0;
	}
	void setFirstCamera(){
		//songo->getReferencedPosition(), songo->getReferencedRotation()
        camera.reset();
        camera = make_shared<CFirstPersonCamera>();
		camera->setPosition(createPositionVector(songo->getPositionXZ(),10));
		camera->setPitch(5.6);
		camera->setYaw(94);
	}
	void setThridCamera(){
        camera.reset();
        camera = make_shared<CThirdPersonCamera>(songo->getReferencedPosition(),songo->getReferencedRotation());
	}
	int update(SDL_Event &event,SDL_Window *win) override{
		camera->move(win);
		songo->calculateMove(0.02);			 
		songo->move(0.02, getHeightOfTerrain(songo->getPositionXZ()));

		if (gui.guiButtons[0].isPressedButton(glm::vec2(1000, 600)))
			return 2;

//		m9->setPosition(songo->getPosition() + glm::vec3(0, 7.5, 0));
//		m9->setRotation(songo->getRotation()+ glm::vec3(camera->getPitch(),0,0));

		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_F1: cout << "Camera position : " << to_string(camera->getPosition()); break;
			case SDLK_F2: cout << "Camera angles : " << camera->getPitch() << " " << camera->getYaw() << " " << camera->getRoll() << endl; break;
            case SDLK_F3: thridCamera = !thridCamera; thridCamera ? setThridCamera() : setFirstCamera(); break;
			}
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y == 1)
			{
				camera->calculateZoom(-1);
			}
			else if (event.wheel.y == -1)
			{
				camera->calculateZoom(1);
			}break;
		
		break;
		}
		event = SDL_Event();
		return 0;
	}
	int cleanUp() override{
		loader.cleanUp();
		for (int x = 0; x < terrains.size(); x++) {
			terrains[x].cleanUp();
		}
		terrains.clear();
		entities.clear();
		gui.guiButtons.clear();
		gui.guiTexts.clear();
		gui.guiTextures.clear();
		lights.clear();
		camera.reset();
		return 0;
	}
	glm::mat4 getViewMatrix() { return camera->getViewMatrix(); }


};
#endif // !SCENE_H


