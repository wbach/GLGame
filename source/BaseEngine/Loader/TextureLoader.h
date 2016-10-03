#pragma once
#include <string>
#include <vector>
#include <FreeImage.h>
#include "Material.h"

class CTextureLoader
{
public:
	CTextureLoader(std::vector<STextInfo>& textures_vector);
	GLuint LoadFullTexture(string filename, bool keep_data, GLubyte *&data, int &width, int &height, bool vertical_flip, float quality = 1);
	GLuint LoadTexture(string filename, bool verticalFlip = false);
	GLuint LoadCubeMap(const vector<string>& filenames);
private:
	std::vector<STextInfo>& m_Textures;
};