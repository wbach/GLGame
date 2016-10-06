#include "SceneEditor.h"

CSceneEditor::CSceneEditor(CGame& game)
	: m_Game(game)
	, m_CurrentEntity(nullptr)
{
	m_Instance = GetModuleHandle(NULL);

	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	m_CurrentPath = buffer;
	m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.find_last_of('\\'));
	m_CurrentPath += '\\';
}

void CSceneEditor::Init(int width, int height)
{
	m_Width = width;
	m_Height = height;

	InitCommonControls();

	// Create Editro window
	CreateWndClass();
	CreateMainWindow();
	
	hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	// Attach OpenGL window
	SetWindowLongPtr(m_Hwnd[Hwnds::MAIN_WINDOW], GWLP_USERDATA, (long)this);
	AttachOpenGLWindow();
	
	// file explorer init
	CreateFileList();	
	FindFilesInDirectory(m_CurrentPath);
	CheckAvaibleDrives();
	
	// Objects Tree init
	CreateObjectsTree();
	FillObjectsTree();

	//Create Inspector
	CreateInspector(m_Width - 175, 5);

	// Setfont to all elements
	for (int x = 0; x < Hwnds::COUNT; x++)
		SendMessage(m_Hwnd[x], WM_SETFONT, (WPARAM)hNormalFont, 0);	

	ShowWindow(m_Hwnd[Hwnds::MAIN_WINDOW], SW_SHOWDEFAULT); // Poka¿ okienko...
	UpdateWindow(m_Hwnd[Hwnds::MAIN_WINDOW]);
}
void CSceneEditor::AddMenus() 
{
	m_Menus[MenuHandles::POUP] = CreateMenu();
	m_Menus[MenuHandles::ELEMENT_FILE] = CreateMenu();
	m_Menus[MenuHandles::ELEMENT_HELP] = CreateMenu();

	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_STRING, ControlsIds::MENU_NEW_SCENE, L"&New");
	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_STRING, ControlsIds::MENU_OPEN_SCENE, L"&Open");
	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_SEPARATOR, 0, NULL);
	AppendMenuW(m_Menus[MenuHandles::ELEMENT_FILE], MF_STRING, ControlsIds::MENU_QUIT, L"&Quit");

	AppendMenuW(m_Menus[MenuHandles::ELEMENT_HELP], MF_STRING, ControlsIds::MENU_ABOUT, L"&About");

	AppendMenuW(m_Menus[MenuHandles::POUP], MF_POPUP, (UINT_PTR) m_Menus[MenuHandles::ELEMENT_FILE], L"&File");
	AppendMenuW(m_Menus[MenuHandles::POUP], MF_POPUP, (UINT_PTR) m_Menus[MenuHandles::ELEMENT_HELP], L"&About");

	

	SetMenu(m_Hwnd[Hwnds::MAIN_WINDOW], m_Menus[MenuHandles::POUP]);

	//HMENU menu = m_Menus[MenuHandles::ELEMENT_FILE];
	//MENUINFO MenuInfo = { 0 };
	//MenuInfo.cbSize = sizeof(MenuInfo);
	//MenuInfo.hbrBack = CreateSolidBrush(RGB(30, 30, 30)); // Brush you want to draw
	//MenuInfo.fMask = MIM_BACKGROUND;
	//MenuInfo.dwStyle = MNS_AUTODISMISS;

	//if (IsMenu(menu) )
	//{
	//	SetMenuInfo(menu, &MenuInfo);
	//}

}
void CSceneEditor::CreateMainWindow()
{
	// create window
	m_Hwnd[Hwnds::MAIN_WINDOW] = CreateWindowEx(WS_EX_CLIENTEDGE, m_ClassName, "Editor", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, m_Width, m_Height, NULL, NULL, m_Instance, NULL);

	if (m_Hwnd[Hwnds::MAIN_WINDOW] == NULL)
	{
		MessageBox(NULL, "Cant create editor window.", "Editor", MB_ICONEXCLAMATION);
		return;
	}
	AddMenus();
}

void CSceneEditor::AttachOpenGLWindow()
{
	m_Hwnd[Hwnds::OPENGL_WINDOW] = FindWindow(NULL, "Gota - OpenGL");
	SetParent(m_Hwnd[Hwnds::OPENGL_WINDOW], m_Hwnd[Hwnds::MAIN_WINDOW]);
	SetWindowPos(m_Hwnd[Hwnds::OPENGL_WINDOW], 0, m_Width / 2 - 320, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void CSceneEditor::CreateWndClass()
{
	// Create wnd class
	m_Wc.cbSize = sizeof(WNDCLASSEX);
	m_Wc.style = 0;
	m_Wc.lpfnWndProc = WndProc;
	m_Wc.cbClsExtra = 0;
	m_Wc.cbWndExtra = 0;
	m_Wc.hInstance = m_Instance;
	m_Wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_Wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_Wc.hbrBackground =(HBRUSH)(COLOR_WINDOW + 1);// CreateSolidBrush(RGB(30, 30, 30));//
	m_Wc.lpszMenuName = NULL;
	m_Wc.lpszClassName = m_ClassName;
	m_Wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// REJESTROWANIE KLASY OKNA
	if (!RegisterClassEx(&m_Wc))
	{
		MessageBox(NULL, "Cant register window", "Error",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}
}
std::string CSceneEditor::GetTextFromControl(HWND hwnd)
{
	DWORD dlugosc = GetWindowTextLength(hwnd);
	LPSTR Bufor = (LPSTR)GlobalAlloc(GPTR, dlugosc + 1);
	GetWindowText(hwnd, Bufor, dlugosc + 1);
	return std::string(Bufor);
}
string FloatToString3(float x)
{
	char text[20];
	sprintf_s(text, "%.3f", x);
	return string(text);
}
void CSceneEditor::UpdateValueControl(HWND hwnd, float x)
{
	HWND focus = GetFocus();
	if (FloatToString3(x).compare(GetTextFromControl(hwnd)) != 0 && focus != hwnd)
		SetWindowText(hwnd, (LPCSTR)FloatToString3(x).c_str());
}
void CSceneEditor::PeekMesages()
{	
	if (m_CurrentEntity != nullptr)
	{
		UpdateInspector();
	}
}
