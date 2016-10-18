#pragma once
#include "GUITexture.h"
#include "GUIText.h"
#include "GUIButton.h"
#include "Font.h"
#include "FontShader.h"
#include <vector>
struct SGUI {
	vector<CGUIButton> guiButtons;
	vector<CGUITexture> guiTextures;
	vector<CGUIText> guiTexts;	
};