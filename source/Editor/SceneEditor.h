#pragma once
#include "../../resource.h"
#include <Windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <iostream>
#include <strsafe.h>
#include <GL/glew.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include "../BaseEngine/Engine/GLgame.h"
namespace ControlsIds
{
	enum Ids 
	{
		FILE_LIST = 1001,
		DRIVE_COMBO,
		OBJECT_TREEE,
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
		FILE_LIST,
		DRIVE_COMBO,
		OBJECT_TREEE,
		CURRENT_PATH,
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
//LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
class CSceneEditor
{
public:
	CSceneEditor(CGame& game);
	void CreateMainWindow(int width, int height);
	void CreateFileList();
	void AttachOpenGLWindow();
	void FillFileList();
	void AddFile(int i, const SFile& file);
	void FindFilesInDirectory(std::string path);
	void PeekMesages();
	void SpawnEntity(std::string file_name);
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void RecursiveSetEntity(const std::shared_ptr<CEntity>& enity, HTREEITEM root);
	void FillObjectsTree();
private:
	void CreateObjectsTree();
	
	void CheckAvaibleDrives();
	LRESULT CALLBACK realWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	std::string m_CurrentPath;
	std::vector<SFile> m_CurrentPathFiles;
	std::vector<std::string> m_Drives;

	int m_Width, m_Height;
	HDC			hDC = NULL;		// Private GDI Device Context
	HGLRC		hRC = NULL;		// Permanent Rendering Context

	MSG m_Messages;

	LPSTR NazwaKlasy = "Klasa Okienka";
	HWND m_Hwnd[Hwnds::COUNT];
	WNDCLASSEX wc;
	HINSTANCE m_Instance;
	int m_nCmdShow;
	HFONT hNormalFont;

	CGame& m_Game;
};