#ifndef GUI_H
#define GUI_H
#include "GUITexture.h"
#include "GUIText.h"
#include "GUIButton.h"
#include "FreeType.h"
#include "FontShader.h"
#include <vector>
using namespace std;
struct CGUI {
	vector<CGUIButton> guiButtons;
	vector<CGUITexture> guiTextures;
	vector<CGUIText>guiTexts;
	CGUI() {}		

};
#endif