#include "SceneEditor.h"

void CSceneEditor::CreateObjectsTree()
{
	m_Hwnd[Hwnds::OBJECT_TREEE] = CreateWindowEx(WS_EX_CLIENTEDGE, WC_TREEVIEW, "Drzefko",
		WS_CHILD | WS_VISIBLE | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT,
		5, 0, m_Width / 2 - 330, 450, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::OBJECT_TREEE, m_Instance, NULL);

}
void CSceneEditor::FillObjectsTree()
{
	TreeView_DeleteAllItems(m_Hwnd[Hwnds::OBJECT_TREEE]);

	TVITEM tvi;
	tvi.mask = TVIF_TEXT;
	tvi.pszText = "Element";

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

void CSceneEditor::RecursiveSetEntity(const std::shared_ptr<CEntity>& enity, HTREEITEM root)
{
	TVITEM tvi;
	tvi.mask = TVIF_TEXT;
	tvi.pszText = "Element";

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
bool is_number(const std::string& s)
{
	return !s.empty() && std::find_if(s.begin(),
		s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}
void CSceneEditor::TreeProcedure(WPARAM wParam, LPARAM lParam)
{
	switch (((LPNMHDR)lParam)->code)
	{
	case TVN_GETDISPINFO:
	{
		
	}
	break;

	case TVN_SELCHANGED:
	{
		HTREEITEM hCurrentItem = TreeView_GetSelection(m_Hwnd[Hwnds::OBJECT_TREEE]);
		if (hCurrentItem == NULL) // Nothing selected
			break;
		// Now get the text of the selected item
		TCHAR buffer[128];

		TVITEM item;
		item.hItem = hCurrentItem;
		item.mask = TVIF_TEXT;
		item.cchTextMax = 128;
		item.pszText = buffer;
		if (TreeView_GetItem(m_Hwnd[Hwnds::OBJECT_TREEE], &item))
		{
			string text(item.pszText);
			std::string a_id = text.substr(text.find_last_of('_') + 1);
			cout << "Type : " << a_id[0] << endl;
			std::string id = a_id.substr(a_id.find_first_not_of(a_id[0]));
			cout << "Id : " << id << endl;

			switch (a_id[0])
			{
			case 't':
				break;
			case 'e':
				if (is_number(id))
				{
					m_CurrentEntity = m_Game.GetCurrentScene()->FindEntity(stoi(id));
					PostMessage(m_Hwnd[Hwnds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT], BM_SETCHECK, BST_UNCHECKED, 0);
					m_AttachToTerrainHeight = false;
				}
				break;
			}
			//cout << item.pszText << " is selected" << endl;

		}

	}
	break;

	}
}
