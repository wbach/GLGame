#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <stdlib.h>
#include "../Utils/Utils.h"
#include "TextureLoader.h"
#include "Model.h"
#include "AssimpModel.h"
#include "FbxModel.h"

using namespace std;

class CLoader
{
public:
	CLoader();
	vector<shared_ptr<CModel>>	m_Models;
	vector<STextInfo>			m_Textures;

	int LoadMesh(string file_name);
	GLuint LoadTexture(string file_name, bool vertical_flip = false);
	void CleanUp();

private:
	CTextureLoader	m_TextureLoader;
};

