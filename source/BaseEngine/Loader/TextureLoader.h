#pragma once
#include <string>
#include <vector>
#include <FreeImage.h>
#include "Material.h"
#include <algorithm>
class CTextureLoader
{
public:
	CTextureLoader(std::vector<STextInfo>& textures_vector);
	void ReadImage(string filename, GLubyte *&data, int &width, int &height, bool vertical_flip = false, bool horizontal_flip = false);
	GLuint LoadFullTexture(string filename, bool keep_data, GLubyte *&data, int &width, int &height, bool vertical_flip = false, bool horizontal_flip = false);
	GLuint LoadTexture(string filename, bool verticalFlip = false);
	GLuint LoadCubeMap(const vector<string>& filenames);
	void CreateEmptyImage(const std::string filename, int width, int height);
	void ReloadTexture(string file_name, GLuint* texture_id);
	void ReloadTexture(GLubyte *data, GLuint& texture_id, int width, int height);
	void UpdateSubTexture(GLuint& texture_id, GLubyte* subdata, int start_x, int start_y, int width, int height);
	void SaveTextureToFile(string file_name, GLubyte *data, int width, int height);

	void SetMaxTextureResolution(const glm::vec2& resolution);
private:
	std::vector<STextInfo>& m_Textures;
	glm::vec2 m_MaxTextureResolution;
};