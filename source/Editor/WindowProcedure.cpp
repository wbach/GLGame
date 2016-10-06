#include "SceneEditor.h"


LRESULT CSceneEditor::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	CSceneEditor* me = (CSceneEditor*)(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	if (me) return me->RealWndProc(hwnd, msg, wParam, lParam);
	return DefWindowProc(hwnd, msg, wParam, lParam);
}



LRESULT CSceneEditor::RealWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		m_Game.m_FroceQuit = true;
		DestroyWindow(hwnd);
		break;
	case WM_NOTIFY:
		FileExplorerProcedure(wParam, lParam);	
		TreeProcedure(wParam,lParam);
		break;
	case WM_COMMAND:
		InspectorProcedure(LOWORD(wParam), HIWORD(wParam));
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
		case ControlsIds::MENU_NEW_SCENE:
			break;		
		case  ControlsIds::MENU_OPEN_SCENE:
			MessageBeep(MB_ICONINFORMATION);
			break;
		case  ControlsIds::MENU_QUIT:
			m_Game.m_FroceQuit = true;
			SendMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case ControlsIds::MENU_ABOUT:

			break;
		}
		break;
	case WM_DESTROY:
		m_Game.m_FroceQuit = true;
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}
