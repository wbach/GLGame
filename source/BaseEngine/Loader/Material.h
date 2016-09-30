#pragma once
#include <string>
#include "glm/glm.hpp"
#include "../Utils/Utils.h"
using namespace std;
namespace MaterialTexture 
{
	enum {
		AMBIENT = 0,
		DIFFUSE,
		SPECULAR,
		NORMAL,
		REFLECTION,
		BLEND_MAP,
		GUI
	};
}

struct STextInfo 
{
	GLuint id;
	string filename;
	unsigned int type;
	STextInfo()
	{
		id = 0;
		type = 0;
	}
	void CleanUp() 
	{
		glDeleteTextures(1, &id);
	}
};
struct SMaterial
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shineDamper;
	float reflectivity;
	vector<STextInfo> textures;
	bool isTransparency;
	bool useFakeLighting;
	int numberOfRows;
	int textureIndex;

	SMaterial()
	{
		textureIndex = 0;
		numberOfRows = 1;
		useFakeLighting = false;
		isTransparency = false;
		ambient = specular = glm::vec3(0);
		diffuse = glm::vec3(0.8);
	}
	float getTextureXOffset() 
	{
		int column = textureIndex%numberOfRows;
		return (float)column / (float)numberOfRows;
	}
	float getTextureYOffset()
	{
		int row = textureIndex / numberOfRows;
		return (float)row / (float)numberOfRows;
	}
};
