#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <stdlib.h>
#include "../Utils/Utils.h"
#include "TextureLoader.h"
#include "Model.h"
#include "AssimpModel.h"
#include "FbxModel.h"
#include <thread>
using namespace std;

class CLoader
{
public:
	CLoader();
	vector<shared_ptr<CModel>>	m_Models;
	vector<STextInfo>			m_Textures;
	
	void	AddModelToUpdate(const unsigned int& eid, const unsigned int& mid);
	void	UpdateModels(float delta_time);	
	void    CreateUpdateThread();
	void	UpdateModelsThread();
	int		LoadMesh(string file_name, bool time_update = false);
	GLuint	LoadTexture(string file_name, bool vertical_flip = false);
	GLuint	LoadFullTexture(string file_name, GLubyte *&data, int &width, int &height);
	GLuint	LoadCubeMap(const vector<string>& filenames);
	void	CreateEmptyImage(const std::string filename, int width, int height);
	void	ReloadTexture(string file_name, GLuint& texture_id);
	void	ReloadTexture(GLubyte *data, GLuint& texture_id, int width, int height);
	void	UpdateSubTexture(GLuint& texture_id, GLubyte* subdata, int start_x, int start_y, int width, int height);
	std::vector<SFace> LoadFaces(const std::string& filename) const;
	CTextureLoader& GetTextureLoader();
	void	SaveTextureToFile(string file_name, GLubyte *data, int width, int height);
	void	CleanUp();

	void	SetMaxTextureResolution(const glm::vec2& resolution);
private:
	CTextureLoader	m_TextureLoader;
	unordered_map<int, int>   m_IndexesInFrame;
	vector<int>	m_IndexesUpdatingModels;
	std::thread animation_thread;
	bool m_UpdateThreadRun;
};

