#pragma once

#include <string>
#include <vector>
#include "../BaseEngine/Engine/Scene.h"
#include "../BaseEngine/Camera/ThridPersonCamera.h"
using namespace std;
class CTestMainMenu: public CScene
{
public:
	CTestMainMenu()  {
		m_Name = "Test Main menu scene";
		CGUITexture background(m_Loader.loadTexture("Data/GUI/mmbackground.jpg"),glm::vec2(0.0),glm::vec2(1.0));		
		m_Gui.guiTextures.push_back(background);
	}
	int Initialize() override{
		CGUIButton startButton(m_Loader.loadTexture("Data/GUI/startGameButton.png"), m_Loader.loadTexture("Data/GUI/hoverStartGameButton.png"), m_Loader.loadTexture("Data/GUI/pushStartGamebutton.png"), "test", glm::vec2(-0.5,0.25), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		CGUIButton exitButton(m_Loader.loadTexture("Data/GUI/exitGameButton.png"), m_Loader.loadTexture("Data/GUI/exitGameHoverBUtton.png"), m_Loader.loadTexture("Data/GUI/exitGamePushButton.png"), "test", glm::vec2(-0.5, 0.1), 10, glm::vec3(1), glm::vec2(0.1, 0.05));
		m_Gui.guiButtons.push_back(startButton);
		m_Gui.guiButtons.push_back(exitButton);
		return 0;
	}
	int Update(SDL_Event &event, SDL_Window *win) override {
		if (m_Gui.guiButtons[0].CheckStatus(glm::vec2(1000, 600)) == GuiButtonState::ACTIVE) {
			cout << "Pressed button 0... " << endl;
			return 2;
		}
			
		if (m_Gui.guiButtons[1].CheckStatus(glm::vec2(1000, 600)) == GuiButtonState::ACTIVE) {
			cout << "Pressed button 1... " << endl;
			return 1;
		}
			
		return 0;
	}
	int CleanUp() override
	{
		m_Loader.cleanUp();
		return 0;
	}
	const glm::mat4& getViewMatrix() { return m_Camera->GetViewMatrix(); }
};


