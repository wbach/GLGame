#ifndef LOADER_H
#define LOADER_H
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Model.h"
#include "UTIL.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdlib.h>
#include <FreeImage.h>
using namespace std;
class CLoader {
public:
	vector<CModel> models;
	vector<TextInfo> textures;

	int assimpLoad(string filename);

	GLuint loadFullTexture(string filename, bool keepData, GLubyte *&data, int &width, int &height, bool verticalFlip);
	GLuint loadTexture(string filename,bool verticalFlip = false);
	void updateTexture(GLuint id, string filename);
	GLuint loadCubeMap(vector<string> filenames);

	void processMesh(CModel &model,string filepath, aiMesh *mesh, const aiScene *scene);
	void recursiveProcess(CModel &model,string filepath, aiNode *node, const aiScene *scene);

	void cleanUp();
};
#endif // !ENTITY_H


