#include "Editor/SceneEditor.h"
#include "BaseEngine/Engine/GLgame.h"
#include "BaseEngine/APIs/GLFW3/GLFW3_OpenGL.h"
#include "BaseEngine/APIs/SDL2/SDLOpenGL.h"
#include "TestGame/Test_Scene.h"
#include <map>

#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


CGame myGame;
#ifdef EDITOR
CSceneEditor editor(myGame);
#endif
template<typename T> CScene* createInstance() { return new T; }
typedef std::map<std::string, CScene*(*)()> scenes_type;

int main(int argc, char *argv[])
{	
	//Test git cooment
	//char Title[512]; ZeroMemory(Title, 512); GetConsoleTitle(Title, 512);	HWND console = FindWindow(NULL, Title); ShowWindow(console, NULL);
	//  shared_ptr<CScene> mainMenu = make_shared<CTestMainMenu>(myGame);

	//myGame.addScene(mainMenu);

	//std::ofstream file;
	//file.open("Data/Terrain/FloatTerrain32x32Empty.terrain");
	//file << "r32x32" << endl;
	//for (int y = 0; y < 32; y++)
	//{
	//	for (int x = 0; x < 32; x++)
	//	{
	//		file << 0 << " ";
	//	}
	//	file << endl;
	//}
	//file.close();	

	int test_scene_camera_type = 0;
	std::shared_ptr<CApi> api;
#ifdef EDITOR
	myGame.SetWindowSize(glm::vec2(820, 480));
	api = std::make_shared<CSdlOpenGlApi>();
	test_scene_camera_type = 1;
#else
	//api = std::make_shared<CGlfwOpenGlApi>();
	api = std::make_shared<CSdlOpenGlApi>();
#endif
	
	myGame.AddScene(std::make_shared<CTestSCene>(myGame, test_scene_camera_type));
	if (myGame.SetCurrentScene(0) < 0)
	{
		cout << "[Error] Scene not found." << endl;
	}
	myGame.Initialize(api);  	
#ifdef EDITOR	
	//std::thread  a(&CSceneEditor::Init, editor, 1366, 768);
	editor.Init(1366, 768);
	editor.PeekMesages();
	myGame.OnGameLoopRun = []{editor.PeekMesages(); };
	myGame.m_SceneParser.LoadScene(myGame.GetCurrentScene()->m_SceneFile, myGame.GetCurrentScene());
#else
	myGame.m_SceneParser.LoadScene(myGame.GetCurrentScene()->m_SceneFile, myGame.GetCurrentScene());
	myGame.GetCurrentScene()->PostInitialize();
#endif
    myGame.GameLoop();
	//a.detach();
    myGame.Uninitialize();

	_CrtDumpMemoryLeaks();
	return 0;
}
