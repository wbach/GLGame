#pragma once
#include <Windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <iostream>
#include <strsafe.h>
#include <GL/glew.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <cctype>
#include "../BaseEngine/Engine/GLgame.h"
#include <thread>
#define TEXTCOLOR RGB(0, 0, 0)
#define BKCOLOR RGB(250, 250, 250)
#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

static HWND s_ProgresBarLoading;
static HWND s_DialogLoading;

namespace ControlsIds
{
	enum Ids 
	{
				//File list
		FILE_LIST = 1001,
		DRIVE_COMBO,
		//Inspector
		INSPECTOR_EDIT_POSITION_X,
		INSPECTOR_EDIT_POSITION_Y,
		INSPECTOR_EDIT_POSITION_Z,
		INSPECTOR_EDIT_ROTATION_X,
		INSPECTOR_EDIT_ROTATION_Y,
		INSPECTOR_EDIT_ROTATION_Z,
		INSPECTOR_EDIT_SCALE_X,
		INSPECTOR_EDIT_SCALE_Y,
		INSPECTOR_EDIT_SCALE_Z,
		INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT,
		INSPECTOR_DELETE_CURRENT_SELECTED,
		INSPECTOR_TEXT_CURRENT_SELECTED,
		INSPECTOR_GO_CAMERA_TO_OBJECT,
		INSPECTOR_TEXTURE_BACKGROUNG,
		INSPECTOR_TEXTURE_RED,
		INSPECTOR_TEXTURE_GREEN,
		INSPECTOR_TEXTURE_BLUE,
		INSPECTOR_PAINT_SIZE,
		INSPECTOR_PAINT_STRENGTH,
		INSPECTOR_PAINT_SIZE_TEXT,
		INSPECTOR_PAINT_STRENGTH_TEXT,
		//Tree
		OBJECT_TREEE,
		//Menu
		MENU_NEW_SCENE,
		MENU_OPEN_SCENE,
		MENU_SAVE_SCENE,
		MENU_SAVEAS_SCENE,
		MENU_ABOUT,
		MENU_QUIT,
		//Count
		COUNT
	};
}
namespace FilesTypes 
{
	enum Type
	{
		NONE = 0,
		DIRECTORY,
		DEFAULT,
		IMAGE,
		COUNT
	};
}
namespace Hwnds
{
	enum Type
	{
		MAIN_WINDOW = 0,
		OPENGL_WINDOW,
		//Inscpector
		INSPECTOR_TEXT_POSITION,
		INSPECTOR_TEXT_ROTATION,
		INSPECTOR_TEXT_SCALE,
		INSPECTOR_EDIT_POSITION_X,
		INSPECTOR_EDIT_POSITION_Y,
		INSPECTOR_EDIT_POSITION_Z,
		INSPECTOR_EDIT_ROTATION_X,
		INSPECTOR_EDIT_ROTATION_Y,
		INSPECTOR_EDIT_ROTATION_Z,
		INSPECTOR_EDIT_SCALE_X,
		INSPECTOR_EDIT_SCALE_Y,
		INSPECTOR_EDIT_SCALE_Z,
		INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT,
		INSPECTOR_DELETE_CURRENT_SELECTED,
		INSPECTOR_TEXT_CURRENT_SELECTED,
		INSPECTOR_GO_CAMERA_TO_OBJECT,
		INSPECTOR_TEXTURE_BACKGROUNG,
		INSPECTOR_TEXTURE_RED,
		INSPECTOR_TEXTURE_GREEN,
		INSPECTOR_TEXTURE_BLUE,
		INSPECTOR_PAINT_SIZE,
		INSPECTOR_PAINT_STRENGTH,
		INSPECTOR_PAINT_SIZE_TEXT,
		INSPECTOR_PAINT_STRENGTH_TEXT,
		//File list
		FILE_LIST,
		DRIVE_COMBO,
		CURRENT_PATH,
		//Tree
		OBJECT_TREEE,		
		//Menu
		MENU,
		MENU_BAR,
		//
		LOADING_DIALOG,
		LOADING_PROGRESS_BAR,
		//COunt
		COUNT
	};
}
namespace MenuHandles
{
	enum Type
	{
		POUP = 0,
		ELEMENT_FILE,
		ELEMENT_HELP,
		COUNT
	};
}
struct SFile 
{
	FilesTypes::Type fileType;
	std::string name;
	std::string date;
	long int size;
};

class CSceneEditor
{
public:
	CSceneEditor(CGame& game);
	void Init(int width, int height);	
	void PeekMesages();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);	
private:	
	//
	void CreateDialogProgressBar();
	void DeleteDialogProgressBar();
	void RegisterLoadingClass(HWND hwnd);
	static LRESULT CALLBACK LoadingDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	std::string GetTextFromControl(HWND hwnd);


	// Inspector function
	void CreateInspector(int w, int h);
	void CreateTerrainTextureView(int &w, int &h, int size_x, int size_y);
	void CreateTransformView(int &w, int &h, int size_x, int size_y);
	void UpdateValueControl(HWND hwnd, float x);
	void InspectorProcedure(WPARAM wParam, LPARAM lParam);
	void UpdateInspector();
	void UpdateEntity(ControlsIds::Ids type);
	void GetValueFromControl(HWND hwnd, float& value);


	// Objects Tree functions
	void CreateObjectsTree();
	void FillObjectsTree();
	void RecursiveSetEntity(const std::shared_ptr<CEntity>& enity, HTREEITEM root);
	void TreeProcedure(HWND hwnd, WPARAM wParam, LPARAM lParam);

	// File Expllorer functions
	void CheckAvaibleDrives();
	void FindFilesInDirectory(std::string path);
	void FillFileList();
	void CreateFileList();
	void AddFile(int i, const SFile& file);
	void SpawnEntity(std::string file_name);
	void FileExplorerProcedure(WPARAM wParam, LPARAM lParam);

	// Window functions
	void AttachOpenGLWindow();	
	void CreateWndClass();
	void CreateMainWindow();
	void AddMenus();
	void CheckActiveWindows();
	LRESULT CALLBACK RealWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//*
	void DisplayBitmap(HWND hwnd, HBITMAP bitmap, HDC hdc, int x, int y);
	static HBITMAP Create8bppBitmap(HDC hdc, int width, int height, LPVOID pBits = NULL);
	static HBITMAP CreateBitmapFromPixels(HDC hDC, UINT uWidth, UINT uHeight, UINT uBitsPerPixel, LPVOID pBits);

	// Inspector variables
	bool m_AttachToTerrainHeight = false;
	shared_ptr<CEntity> m_CurrentEntity = nullptr;
	CTerrain* m_CurrentTerrain = nullptr;

	int m_BrushSize = 50;
	float m_PaintStrength = 1.f;

	// File Expllorer variables
	std::string				 m_CurrentPath;
	std::vector<SFile>		 m_CurrentPathFiles;
	std::vector<std::string> m_Drives;

	// Window variables
	int			m_Width;
	int			m_Height;
	MSG			m_Messages;
	LPSTR		m_ClassName = "Window class";
	HWND		m_Hwnd[Hwnds::COUNT];
	WNDCLASSEX	m_Wc;
	HINSTANCE	m_Instance;
	HMENU		m_Menus[MenuHandles::COUNT];
	// Fonts
	HFONT hNormalFont;
	// Game
	CGame& m_Game;
};