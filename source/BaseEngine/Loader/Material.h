#ifndef MATERIAL_H
#define MATERIAL_H
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
	STextInfo() {
		id = 0;
		type = 0;
	}
	void CleanUp() {
		glDeleteTextures(1, &id);
	}
};
struct SMaterial
{
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shineDamper;
	float reflectivity;
	vector<STextInfo> textures;
	bool isTransparency;
	bool useFakeLighting;
	int numberOfRows;
	int textureIndex;

	SMaterial() {
		textureIndex = 0;
		numberOfRows = 1;
		useFakeLighting = false;
		isTransparency = false;
	}
	float getTextureXOffset() {
		int column = textureIndex%numberOfRows;
		return (float)column / (float)numberOfRows;
	}
	float getTextureYOffset() {
		int row = textureIndex / numberOfRows;
		return (float)row / (float)numberOfRows;
	}
};

#endif // !MATERIAL_H
