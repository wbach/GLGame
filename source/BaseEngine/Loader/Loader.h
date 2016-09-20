#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Model.h"
#include "../Utils/Utils.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdlib.h>
#include <FreeImage.h>
using namespace std;
class CLoader
{
public:
	vector<CModel> m_Models;
	vector<STextInfo> m_Textures;

	int AssimpLoad(string filename);

	GLuint LoadFullTexture(string filename, bool keep_data, GLubyte *&data, int &width, int &height, bool vertical_flip, float quality = 1);
	GLuint LoadTexture(string filename,bool verticalFlip = false);
	void UpdateTexture(GLuint id, string filename);
	GLuint LoadCubeMap(vector<string> filenames);

	void ProcessMesh(CModel &model,string filepath, aiMesh *mesh, const aiScene *scene);
	void RecursiveProcess(CModel &model,string filepath, aiNode *node, const aiScene *scene);

	void CleanUp();
};

