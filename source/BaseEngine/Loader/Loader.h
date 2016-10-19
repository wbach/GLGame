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
	GLuint	LoadFullTexture(string file_name, GLubyte *&data, int &width, int &height);
	GLuint	LoadCubeMap(const vector<string>& filenames);
	void	ReloadTexture(string file_name, GLuint& texture_id);
	void	ReloadTexture(GLubyte *data, GLuint& texture_id, int width, int height);
	void	UpdateSubTexture(GLuint& texture_id, GLubyte* subdata, int start_x, int start_y, int width, int height);
	void	SaveTextureToFile(string file_name, GLubyte *data, int width, int height);
	void	CleanUp();

	void	SetMaxTextureResolution(const glm::vec2& resolution);
private:
	CTextureLoader	m_TextureLoader;
	vector<int>	m_IndexesUpdatingModels;
};

