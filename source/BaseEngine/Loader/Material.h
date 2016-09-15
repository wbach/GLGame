#ifndef MATERIAL_H
#define MATERIAL_H
#pragma once
#include <string>
#include "glm/glm.hpp"
#include "../Utils/Utils.h"
using namespace std;
enum {
	AMBIENT_TEXTURE = 0,
	DIFFUSE_TEXTURE,
	SPECULAR_TEXTURE,
	NORMAL_TEXTURE,
	REFLECTION_TEXTURE,
	BLEND_MAP_TEXTURE,
	GUI_TEXTURE
};
struct TextInfo {
	GLuint id;
	string filename;
	unsigned int type;
	TextInfo() {
		id = 0;
		type = 0;
	}
	void cleanUp() {
		glDeleteTextures(1, &id);
	}
};
struct Material {
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shineDamper;
	float reflectivity;
	vector<TextInfo> textures;
	bool isTransparency;
	bool useFakeLighting;
	int numberOfRows;
	int textureIndex;

	Material() {
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
