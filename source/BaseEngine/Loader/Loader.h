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

	void	UpdateModels(float delta_time);
	int		LoadMesh(string file_name, bool time_update = false);
	GLuint	LoadTexture(string file_name, bool vertical_flip = false);
	GLuint	LoadCubeMap(const vector<string>& filenames);
	void	CleanUp();

private:
	CTextureLoader	m_TextureLoader;
	vector<int>	m_IndexesUpdatingModels;
};

