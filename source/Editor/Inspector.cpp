#include "SceneEditor.h"



void CSceneEditor::CreateInspector(int w, int h)
{
	/*INSPECTOR_TEXT_POSITION,
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
		INSPECTOR_EDIT_SCALE_Z, */
	int edit_size_x = 50, edit_size_y = 20;
	//Position
	m_Hwnd[Hwnds::INSPECTOR_TEXT_POSITION]= CreateWindowEx(0				  , "STATIC", "Positions"	, WS_CHILD | WS_VISIBLE, w					, h					, 150, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_X] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0"				, WS_CHILD | WS_VISIBLE, w					, h + edit_size_y	, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU) ControlsIds::INSPECTOR_EDIT_POSITION_X, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Y] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0"				, WS_CHILD | WS_VISIBLE, w + edit_size_x	, h + edit_size_y	, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_POSITION_Y, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Z] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0"				, WS_CHILD | WS_VISIBLE, w + 2*edit_size_x	, h + edit_size_y	, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_POSITION_Z, m_Instance, NULL);
	
	h += 2 * edit_size_y;
	//Rotation
	m_Hwnd[Hwnds::INSPECTOR_TEXT_ROTATION] = CreateWindowEx(0, "STATIC", "Rotation", WS_CHILD | WS_VISIBLE, w, h, 150, 20, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_X] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU) ControlsIds::INSPECTOR_EDIT_ROTATION_X, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Y] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU) ControlsIds::INSPECTOR_EDIT_ROTATION_Y, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Z] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + 2 * edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU) ControlsIds::INSPECTOR_EDIT_ROTATION_Z, m_Instance, NULL);
	
	//Scale
	h += 2 * edit_size_y;
	//Rotation
	m_Hwnd[Hwnds::INSPECTOR_TEXT_SCALE] = CreateWindowEx(0, "STATIC", "Scale", WS_CHILD | WS_VISIBLE, w, h, 150, 20, m_Hwnd[Hwnds::MAIN_WINDOW], NULL, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_X] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_SCALE_X, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Y] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_SCALE_Y, m_Instance, NULL);
	m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Z] = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "0", WS_CHILD | WS_VISIBLE, w + 2 * edit_size_x, h + edit_size_y, edit_size_x, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_EDIT_SCALE_Z, m_Instance, NULL);

	h += 2 * edit_size_y;

	m_Hwnd[Hwnds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT]  = CreateWindowEx(0, "BUTTON", "Attach to terrain height", WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
		w, h, 150, edit_size_y, m_Hwnd[Hwnds::MAIN_WINDOW], (HMENU)ControlsIds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT, m_Instance, NULL);
}

void CSceneEditor::UpdateInspector()
{
	glm::vec3 position = m_CurrentEntity->GetPosition();
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_X], position.x);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Y], position.y);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Z], position.z);

	glm::vec3 rotation = m_CurrentEntity->GetRotation();
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_X], rotation.x);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Y], rotation.y);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Z], rotation.z);

	glm::vec3 scale = m_CurrentEntity->GetScale();
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_X], scale.x);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Y], scale.y);
	UpdateValueControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Z], scale.z);
}

void CSceneEditor::UpdateEntity(ControlsIds::Ids type)
{
	if (m_CurrentEntity == nullptr)
		return;

	switch (type)
	{
		case ControlsIds::INSPECTOR_EDIT_POSITION_X:
			{
			float x;
				GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_X], x);
				m_CurrentEntity->SetPositionX(x);
			}		
			break;
		case ControlsIds::INSPECTOR_EDIT_POSITION_Y:
		{
			float y;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Y], y);
			m_CurrentEntity->SetPositionY(y);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_POSITION_Z:
		{
			float z;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_POSITION_Z], z);
			m_CurrentEntity->SetPositionZ(z);
		}
		break;

		case ControlsIds::INSPECTOR_EDIT_ROTATION_X:
		{
			float x;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_X], x);
			m_CurrentEntity->SetRotationX(x);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_ROTATION_Y:
		{
			float y;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Y], y);
			m_CurrentEntity->SetRotationY(y);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_ROTATION_Z:
		{
			float z;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_ROTATION_Z], z);
			m_CurrentEntity->SetRotationZ(z);
		}
		break;

		case ControlsIds::INSPECTOR_EDIT_SCALE_X:
		{
			float x;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_X], x);
			m_CurrentEntity->SetScaleX(x);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_SCALE_Y:
		{
			float y;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Y], y);
			m_CurrentEntity->SetScaleY(y);
		}
		break;
		case ControlsIds::INSPECTOR_EDIT_SCALE_Z:
		{
			float z;
			GetValueFromControl(m_Hwnd[Hwnds::INSPECTOR_EDIT_SCALE_Z], z);
			m_CurrentEntity->SetScaleZ(z);
		}
		break;

	}

}
void CSceneEditor::GetValueFromControl(HWND hwnd, float & value)
{
	
	std::string string_value = GetTextFromControl(hwnd);
	if (string_value.size() <= 0) return;
	else if (string_value.size() == 1 && string_value[0] == '.') return;

	if (string_value.find_first_not_of("1234567890.-") != string::npos) return;
	value = stof(string_value);
}

void CSceneEditor::InspectorProcedure(WPARAM wParam, LPARAM lParam)
{
	int nr = ControlsIds::INSPECTOR_EDIT_SCALE_Z - ControlsIds::INSPECTOR_EDIT_POSITION_X;
	for (int x = 0; x < nr +1 ; x++)
	{
		if (wParam == ControlsIds::INSPECTOR_EDIT_POSITION_X + x && EN_CHANGE == lParam)
		{
			UpdateEntity((ControlsIds::Ids)(ControlsIds::INSPECTOR_EDIT_POSITION_X + x));
		}
	}

	switch (wParam)
	{
	case ControlsIds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT:
		{
			m_AttachToTerrainHeight = !m_AttachToTerrainHeight;

			if (m_AttachToTerrainHeight)
				PostMessage(m_Hwnd[Hwnds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT], BM_SETCHECK, BST_CHECKED, 0);
			else
				PostMessage(m_Hwnd[Hwnds::INSPECTOR_ATTACH_TO_TERRAIN_HEIGHT], BM_SETCHECK, BST_UNCHECKED, 0);
		}
		break;
	}
}
