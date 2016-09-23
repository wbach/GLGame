#include "BaseEngine/Engine/GLgame.h"
#include "TestGame/Test_Scene.h"
#include "TestGame/Test_MainMenu.h"

CGame myGame;

int main(int argc, char *argv[]) {

	//Test git cooment
    myGame.Initialize();
    shared_ptr<CScene> mainMenu = make_shared<CTestMainMenu>(myGame);
	shared_ptr<CScene> testScene = make_shared<CTestSCene>(myGame);
	//myGame.addScene(mainMenu);
	myGame.AddScene(testScene);
	if (myGame.SetCurrentScene(0) < 0)
	{
		cout << "Scene not found." << endl;
	}
    myGame.GameLoop();
    myGame.Uninitialize();
	return 0;
}
