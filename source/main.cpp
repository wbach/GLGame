#include "GLGame.h"
#include "Test_Scene.h"
#include "Test_MainMenu.h"
CGame myGame ;

int main(int argc, char *argv[]) {

    myGame.initialize();
    shared_ptr<CScene> mainMenu = make_shared<CTestMainMenu>();
	shared_ptr<CScene> testScene = make_shared<CTestSCene>();
	myGame.addScene(mainMenu);
	myGame.addScene(testScene);
	if (myGame.setCurrentScene(0) < 0)
	{
		cout << "Scene not found." << endl;
	}
    myGame.gameLoop();
    myGame.uninitialize();
	return 0;
}
