#define EDITOR

#ifdef EDITOR
#include "Editor/SceneEditor.h"
#endif

#include "BaseEngine/Engine/GLgame.h"
#include "BaseEngine/APIs/GLFW3/GLFW3_OpenGL.h"
#include "BaseEngine/APIs/SDL2/SDLOpenGL.h"
#include "TestGame/Test_Scene.h"
//#include "TestGame/Test_MainMenu.h"

CGame myGame;
#ifdef EDITOR
CSceneEditor editor(myGame);
#endif

int main(int argc, char *argv[])
{
	

	//Test git cooment
	//char Title[512]; ZeroMemory(Title, 512); GetConsoleTitle(Title, 512);	HWND console = FindWindow(NULL, Title); ShowWindow(console, NULL);
	//  shared_ptr<CScene> mainMenu = make_shared<CTestMainMenu>(myGame);

	//myGame.addScene(mainMenu);
	int test_scene_camera_type = 0;
	std::shared_ptr<CApi> api;
#ifdef EDITOR
	myGame.SetWindowSize(glm::vec2(820, 480));
	api = std::make_shared<CSdlOpenGlApi>();
	test_scene_camera_type = 1;
#else
	api = std::make_shared<CGlfwOpenGlApi>();
#endif
	shared_ptr<CScene> testScene = std::make_shared<CTestSCene>(myGame, test_scene_camera_type);
	myGame.AddScene(testScene);
	if (myGame.SetCurrentScene(0) < 0)
	{
		cout << "Scene not found." << endl;
	}
	myGame.Initialize(api);  

#ifdef EDITOR	
	editor.Init(1366, 768);
	editor.PeekMesages();
	myGame.OnGameLoopRun = []{editor.PeekMesages(); };
#endif

    myGame.GameLoop();
    myGame.Uninitialize();
	return 0;
}
