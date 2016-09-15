#pragma once

#include <string>
#include <vector>
#include "../BaseEngine/Engine/Scene.h"
#include "../BaseEngine/Camera/ThridPersonCamera.h"
using namespace std;
class CTestMainMenu: public CScene {


public:
	CTestMainMenu()  {
		name = "Test Main menu scene";
		CGUITexture background(loader.loadTexture("Data/GUI/mmbackground.jpg"),glm::vec2(0.0),glm::vec2(1.0));		
		gui.guiTextures.push_back(background);
	}
	int initialize() override{
		CGUIButton startButton(loader.loadTexture("Data/GUI/startGameButton.png"), loader.loadTexture("Data/GUI/hoverStartGameButton.png"), loader.loadTexture("Data/GUI/pushStartGamebutton.png"), "test", glm::vec2(-0.5,0.25), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		CGUIButton exitButton(loader.loadTexture("Data/GUI/exitGameButton.png"), loader.loadTexture("Data/GUI/exitGameHoverBUtton.png"), loader.loadTexture("Data/GUI/exitGamePushButton.png"), "test", glm::vec2(-0.5, 0.1), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		gui.guiButtons.push_back(startButton);
		gui.guiButtons.push_back(exitButton);
		return 0;
	}
	int update(SDL_Event &event, SDL_Window *win) override {
		if (gui.guiButtons[0].isPressedButton(glm::vec2(1000, 600))) {
			cout << "Pressed button 0... " << endl;
			return 2;
		}
			
		if (gui.guiButtons[1].isPressedButton(glm::vec2(1000, 600))) {
			cout << "Pressed button 1... " << endl;
			return 1;
		}
			
		return 0;
	}
	int cleanUp() override {
		loader.cleanUp();

		return 0;
	}
	glm::mat4 getViewMatrix() { return camera->GetViewMatrix(); }


};


