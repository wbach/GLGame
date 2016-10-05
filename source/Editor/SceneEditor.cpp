#include "SceneEditor.h"

CSceneEditor::CSceneEditor(CGame& game)
	: m_Game(game)
{
	m_Instance = GetModuleHandle(NULL);

	char buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	m_CurrentPath = buffer;
	m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.find_last_of('\\'));
	m_CurrentPath += '\\';
}

void CSceneEditor::CreateMainWindow(int width, int height)
{
	m_Width = width;
	m_Height = height;

	InitCommonControls();
	// WYPE£NIANIE STRUKTURY
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_Instance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = NazwaKlasy;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// REJESTROWANIE KLASY OKNA
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Wysoka Komisja odmawia rejestracji tego okna!", "Niestety...",
			MB_ICONEXCLAMATION | MB_OK);
		return;
	}

	// TWORZENIE OKNA
	m_Hwnd[Hwnds::MAIN_WINDOW] = CreateWindowEx(WS_EX_CLIENTEDGE, NazwaKlasy, "Oto okienko", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, m_Instance, NULL);

	if (m_Hwnd[Hwnds::MAIN_WINDOW] == NULL)
	{
		MessageBox(NULL, "Okno odmówi³o przyjœcia na œwiat!", "Ale kicha...", MB_ICONEXCLAMATION);
		return;
	}
	hNormalFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

	SetWindowLongPtr(m_Hwnd[Hwnds::MAIN_WINDOW], GWLP_USERDATA, (long)this);
	AttachOpenGLWindow();
	CreateFileList();
	
	FindFilesInDirectory(m_CurrentPath);
	//SetWindowLong(m_Hwnd[Hwnds::OPENGL_WINDOW], GWL_STYLE, WS_CHILD | WS_CLIPCHILDREN);

	CheckAvaibleDrives();
	CreateObjectsTree();
	FillObjectsTree();
	for (int x = 0; x < Hwnds::COUNT; x++)
	{
		SendMessage(m_Hwnd[x], WM_SETFONT, (WPARAM)hNormalFont, 0);
	}

	ShowWindow(m_Hwnd[Hwnds::MAIN_WINDOW], SW_SHOWDEFAULT); // Poka¿ okienko...
	UpdateWindow(m_Hwnd[Hwnds::MAIN_WINDOW]);
}

void CSceneEditor::CreateFileList()
{
	RECT rcl;
	GetClientRect(m_Hwnd[Hwnds::MAIN_WINDOW], &rcl);

	m_Hwnd[Hwnds::DRIVE_COMBO]  = CreateWindowEx(WS_EX_CLIENTEDGE, "COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER |
		CBS_DROPDOWNLIST, 0, 455, 50, 200, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::DRIVE_COMBO, m_Instance, NULL);

	m_Hwnd[Hwnds::CURRENT_PATH] = CreateWindowEx(0, "STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 50, 455, m_Width / 2 - 370, 20,
		m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	SetWindowText(m_Hwnd[Hwnds::CURRENT_PATH], m_CurrentPath.c_str());

	m_Hwnd[Hwnds::FILE_LIST] = CreateWindowEx(0, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT |
		LVS_EDITLABELS, 0, 480, rcl.right - rcl.left, 250,
		m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU) ControlsIds::FILE_LIST, m_Instance, NULL);

	ListView_SetExtendedListViewStyle(m_Hwnd[Hwnds::FILE_LIST], LVS_EX_FULLROWSELECT);

	LVCOLUMN lvc;
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	lvc.iSubItem = 0;
	lvc.cx = 300;
	lvc.pszText = "Nazwa pliku";
	ListView_InsertColumn(m_Hwnd[Hwnds::FILE_LIST], 0, &lvc);

	lvc.iSubItem = 1;
	lvc.cx = 50;
	lvc.pszText = "Rozmiar";
	ListView_InsertColumn(m_Hwnd[Hwnds::FILE_LIST], 1, &lvc);

	lvc.iSubItem = 2;
	lvc.cx = rcl.right - rcl.left - 350;
	lvc.pszText = "Data";
	ListView_InsertColumn(m_Hwnd[Hwnds::FILE_LIST], 2, &lvc);	
}

void CSceneEditor::AttachOpenGLWindow()
{
	m_Hwnd[Hwnds::OPENGL_WINDOW] = FindWindow(NULL, "Gota - OpenGL");
	SetParent(m_Hwnd[Hwnds::OPENGL_WINDOW], m_Hwnd[Hwnds::MAIN_WINDOW]);
	SetWindowPos(m_Hwnd[Hwnds::OPENGL_WINDOW], 0, m_Width / 2 - 320, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

void CSceneEditor::FillFileList()
{
	ListView_DeleteAllItems(m_Hwnd[Hwnds::FILE_LIST]);	

	int i = 0;
	for (const SFile& file : m_CurrentPathFiles)
	{
		AddFile(i, file);
		i++;
	}
}

void CSceneEditor::AddFile(int i, const SFile& file)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT;

	lvi.pszText = (LPSTR)(file.name.c_str());
	lvi.iItem = i;
	lvi.iSubItem = 0;
	ListView_InsertItem(m_Hwnd[Hwnds::FILE_LIST], &lvi);
	std::string size = " ";
	
	if (file.fileType == FilesTypes::DIRECTORY)
		size = "Directory";
	else if (file.fileType == FilesTypes::DEFAULT)
		size = std::to_string(file.size) + " MB";

	ListView_SetItemText(m_Hwnd[Hwnds::FILE_LIST], i, 1, (LPSTR)(size.c_str()));
	ListView_SetItemText(m_Hwnd[Hwnds::FILE_LIST], i, 2, (LPSTR)file.date.c_str());

}

void CSceneEditor::FindFilesInDirectory(std::string path)
{
	SetWindowText(m_Hwnd[Hwnds::CURRENT_PATH], path.c_str());
	m_CurrentPathFiles.clear();
	//SFile parent_file;
	//parent_file.name = "...";
	//parent_file.fileType = FilesTypes::NONE;
	//m_CurrentPathFiles.push_back(parent_file);

	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	TCHAR szDir[MAX_PATH];

	StringCchCopy(szDir, MAX_PATH, path.c_str());
	StringCchCat(szDir, MAX_PATH, TEXT("\\*"));

	hFind = FindFirstFile(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind)
		return;

	do
	{
		SFile file;
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			continue;

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			file.fileType = FilesTypes::DIRECTORY;
			file.name = ffd.cFileName;
		}
		else
		{	
			file.fileType = FilesTypes::DEFAULT;
			file.name = ffd.cFileName;
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			file.size = filesize.QuadPart / 1000000;
			FILETIME  lpCreationTime, lpLastAccessTime, lpLastWriteTime;
			GetFileTime(hFind, &lpCreationTime, &lpLastAccessTime, &lpLastWriteTime);
			SYSTEMTIME time;
			FileTimeToSystemTime(&lpCreationTime, &time);

			file.date = std::to_string(time.wDay) + "." + std::to_string(time.wMonth) + "." + std::to_string(time.wYear);
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
		}
		m_CurrentPathFiles.push_back(file);
	} while (FindNextFile(hFind, &ffd) != 0);
	FillFileList();
}

void CSceneEditor::PeekMesages()
{
	while (PeekMessage(&m_Messages, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_Messages);
		DispatchMessage(&m_Messages);
	}
}
void CSceneEditor::SpawnEntity(std::string file_name)
{
	m_Game.GetCurrentScene()->CreateEntityFromFile(file_name);
	FillObjectsTree();
}
LRESULT CSceneEditor::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CSceneEditor* me = (CSceneEditor*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (me) return me->realWndProc(hwnd, msg, wParam, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}
void CSceneEditor::CreateObjectsTree()
{
	m_Hwnd[Hwnds::OBJECT_TREEE] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TREEVIEW, "Drzefko",
		WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
		5, 0, m_Width / 2 - 330, 450, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU) ControlsIds::OBJECT_TREEE, m_Instance, NULL);


	//TVITEM tvi;
	//tvi.mask = TVIF_TEXT;
	//tvi.pszText = "Element Jeden";

	//TVINSERTSTRUCT tvins;
	//tvins.item = tvi;
	//tvins.hParent = tvins.hInsertAfter = TVI_ROOT;

	//HTREEITEM hItem1 = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

	//tvi.pszText = "Element Dwa";
	//tvins.item = tvi;
	//tvins.hInsertAfter = TVI_ROOT; // ...albo hItem1HTREEITEM
	//HTREEITEM hItem2 = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

	//tvi.pszText = "Element Trzy";
	//tvins.item = tvi;
	//tvins.hInsertAfter = TVI_LAST; // ...albo hItem2HTREEITEM
	//HTREEITEM hItem3 = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

	//tvi.pszText = "Dziecko Elementu Jeden";
	//tvins.item = tvi;
	//tvins.hParent = hItem1;
	//tvins.hInsertAfter = TVI_FIRST; // ...albo TVI_LASTHTREEITEM
	//HTREEITEM hItemChild = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

	//tvi.pszText = "Dziecko Elementu Jeden";
	//tvins.item = tvi;
	//tvins.hParent = hItemChild;
	//tvins.hInsertAfter = TVI_FIRST; // ...albo TVI_LASTHTREEITEM
	//HTREEITEM hItemChild2 = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);
}
void CSceneEditor::RecursiveSetEntity(const std::shared_ptr<CEntity>& enity, HTREEITEM root)
{
	TVITEM tvi;
	tvi.mask = TVIF_TEXT;
	tvi.pszText = "Element Jeden";

	TVINSERTSTRUCT tvins;
	tvins.item = tvi;

	tvi.pszText = (LPSTR)enity->GetName().c_str();
	tvins.item = tvi;
	tvins.hInsertAfter = TVI_FIRST;
	tvins.hParent = root;
	HTREEITEM hItem = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

	for (const std::shared_ptr<CEntity>& child : enity->GetChildrenEntities())
	{
		RecursiveSetEntity(child, hItem);
	}
}
void CSceneEditor::FillObjectsTree()
{
	TreeView_DeleteAllItems(m_Hwnd[Hwnds::OBJECT_TREEE]);

	TVITEM tvi;
	tvi.mask = TVIF_TEXT;
	tvi.pszText = "Element Jeden";

	TVINSERTSTRUCT tvins;
	tvins.item = tvi;
	tvins.hParent = tvins.hInsertAfter = TVI_ROOT;

	bool first = true;
	for (const CTerrain& terrain : m_Game.GetCurrentScene()->GetTerrains())
	{
		cout << terrain.GetName() << " size: " << terrain.m_TerrainEntities.size() << endl;
		tvi.pszText = (LPSTR)terrain.GetName().c_str();
		tvins.item = tvi;
		tvins.hParent = TVI_ROOT;
		if (first)
		{
			tvins.hInsertAfter = TVI_ROOT;
			first = false;
		}
		else
		{
			tvins.hInsertAfter = TVI_LAST;
		}
		HTREEITEM hTerrain = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);

		for (const std::shared_ptr<CEntity>& enity : terrain.m_TerrainEntities)
		{
			RecursiveSetEntity(enity, hTerrain);
		}		
	}

	for (const std::shared_ptr<CEntity>& enity : m_Game.GetCurrentScene()->GetEntities())
	{
		tvi.pszText = (LPSTR)enity->GetName().c_str();
		tvins.item = tvi;
		tvins.hParent = TVI_ROOT;
		if (first)
		{
			tvins.hInsertAfter = TVI_ROOT;
			first = false;
		}
		else
		{
			tvins.hInsertAfter = TVI_LAST;
		}
		HTREEITEM hItem2 = TreeView_InsertItem(m_Hwnd[Hwnds::OBJECT_TREEE], &tvins);
	}
}

void CSceneEditor::CheckAvaibleDrives()
{
	const WORD nBufferSize = 40;
	LPSTR Bufor = (LPSTR)GlobalAlloc(GPTR, nBufferSize);
	WORD Wymagane;

	Wymagane = GetLogicalDriveStrings(nBufferSize, Bufor);
	if (Wymagane > nBufferSize) {
		return;
	}
	std::cout << "Dostepne dyski:" << std::endl;
	std::string drive;
	
	for (int x = 0; x < Wymagane; x++)
		if (Bufor[x] != 0) 
		{
			drive += Bufor[x];
		}
		else
		{
			std::cout << drive << std::endl;
			SendMessage(m_Hwnd[Hwnds::DRIVE_COMBO], CB_ADDSTRING, 0, (LPARAM)drive.c_str());
			m_Drives.push_back(drive);
			drive.clear();
		}
	
	int i = 0;
	for (std::string& drive : m_Drives)
	{
		std::string d = m_CurrentPath.substr(0, m_CurrentPath.find_first_of('\\')) + '\\';
		cout << d << " " << drive << endl;
		if (drive.compare(d) == 0)
		{
			SendMessage(m_Hwnd[Hwnds::DRIVE_COMBO], CB_SETCURSEL, 0, (LPARAM) i);
			break;
		}
		i++;
	}
		
	GlobalFree(Bufor);
}
LRESULT CSceneEditor::realWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_NOTIFY:
		switch (wParam)
		{
		case ControlsIds::FILE_LIST:
			//if (((LPNMHDR)lParam)->code == NM_CLICK)
			//{			
			//}
			if (((LPNMHDR)lParam)->code == NM_DBLCLK)
			{
				int index = ListView_GetSelectionMark(m_Hwnd[Hwnds::FILE_LIST]);
				std::string file_all_path;

				int compare_result = m_CurrentPathFiles[index].name.compare("..");
				if (compare_result == 0)
				{
					m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.find_last_of('\\'));
					m_CurrentPath = m_CurrentPath.substr(0, m_CurrentPath.find_last_of('\\'));
					m_CurrentPath+= "\\";
					std::cout << m_CurrentPath << std::endl;
					FindFilesInDirectory(m_CurrentPath);
					break;
				}
				else
				{
					file_all_path = m_CurrentPath + m_CurrentPathFiles[index].name;
				}
				//
				if (m_CurrentPathFiles[index].fileType == FilesTypes::DIRECTORY )
				{
					m_CurrentPath += m_CurrentPathFiles[index].name + "\\";
					FindFilesInDirectory(m_CurrentPath);
				}
				else if (m_CurrentPathFiles[index].fileType == FilesTypes::DEFAULT && compare_result != 0)
				{
					std::string file_extension = m_CurrentPathFiles[index].name.substr(m_CurrentPathFiles[index].name.find_last_of('.') + 1);
					cout << file_extension << endl;
					if (!file_extension.compare("obj"))
					{
						SpawnEntity(file_all_path);
					}
					else
					{
						ShellExecute(NULL, "open", file_all_path.c_str(), NULL, NULL, SW_SHOWDEFAULT);
					}
				}
					
			}
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ControlsIds::DRIVE_COMBO:
			switch (HIWORD(wParam))
			{
			case CBN_DROPDOWN:
					break;
			case BN_CLICKED:
					break;
			case CBN_SELCHANGE:
				//int numer = SendMessage(m_Hwnd[Hwnds::DRIVE_COMBO], CB_GETCURSEL, 0, 0);
				char buf[10];
				GetDlgItemText(hwnd, ControlsIds::DRIVE_COMBO, buf, 9);
				//MessageBox(NULL, buf, NULL, MB_OK);
				m_CurrentPath = std::string(buf);
				FindFilesInDirectory(m_CurrentPath);
				break;
			}
			
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}
