#include "TextureLoader.h"

CTextureLoader::CTextureLoader(std::vector<STextInfo>& textures_vector)
	: m_Textures(textures_vector)
{

}

void CTextureLoader::ReadImage(string file_name, GLubyte *&data, int& width, int& height, float quality , bool vertical_flip, bool horizontal_flip)
{
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(file_name.c_str(), 0);

	if (formato == FIF_UNKNOWN) 
	{ 
		printf("[Error] %s : wrong image format or file does not exist.\n", file_name.c_str());
		return;
	}

	FIBITMAP* imagen2 = FreeImage_Load(formato, file_name.c_str());
	if (!imagen2) 
	{ 
		printf("[Error] %s : wrong image format or file does not exist.\n", file_name.c_str());
		return;
	}

	FIBITMAP* imagen = FreeImage_ConvertTo32Bits(imagen2);
	if (!imagen)
	{
		printf("[Error] %s : cant convert to 32 bits.\n", file_name.c_str());
		return;
	}
//	FreeImage_Unload(imagen2);

	if (vertical_flip)
		FreeImage_FlipVertical(imagen);
	if (horizontal_flip)
		FreeImage_FlipHorizontal(imagen);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	if (quality > 1)
	{
		w = static_cast<int>(w / quality);
		h = static_cast<int>(h / quality);
		imagen = FreeImage_Rescale(imagen, w, h, FILTER_BILINEAR);
	}
	width = w;
	height = h;

	char* pixeles = (char*)FreeImage_GetBits(imagen);

	data = new GLubyte[4 * w*h];

	//bgr2rgb
	for (int j = 0; j<w*h; j++)
	{
		data[j * 4 + 0] = pixeles[j * 4 + 2];
		data[j * 4 + 1] = pixeles[j * 4 + 1];
		data[j * 4 + 2] = pixeles[j * 4 + 0];
		data[j * 4 + 3] = pixeles[j * 4 + 3];
	}
	FreeImage_Unload(imagen);
	FreeImage_Unload(imagen2);
}

GLuint CTextureLoader::LoadFullTexture(string file_name, bool keepData, GLubyte *&texture, int &width, int &height, float quality, bool vertical_flip, bool horizontal_flip )
{
	string file = file_name.substr(file_name.find_last_of('/') + 1);

	for (const STextInfo& t : m_Textures)
	{
		if (file.compare(t.filename) == 0)
			return t.id;
	}

	try
	{
		ReadImage(file_name, texture, width, height, quality, vertical_flip, horizontal_flip);
	}
	catch (int e)
	{
		return 0;
	}

	// Create one OpenGL texture
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	GLenum hubo_error = glGetError();

	if (hubo_error)
	{
		printf("[Error] %s : There was an error loading the texture : %s\n", file_name.c_str(), glewGetErrorString(hubo_error));
		delete[] texture;
		return 0;
	}
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*) texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);

	if (!keepData)
		delete[] texture;

	STextInfo text;
	text.id = texture_id;
	text.filename = file;
	m_Textures.push_back(text);
	return texture_id;
}

GLuint CTextureLoader::LoadTexture(string filename, bool verticalFlip)
{
	int w, h; GLubyte* data = nullptr;
	return LoadFullTexture(filename, false, data, w, h, 1, verticalFlip);
}

GLuint CTextureLoader::LoadCubeMap(const vector<string>& filenames)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
	int i = 0;
	for (string file : filenames)
	{
		GLubyte* data = nullptr;
		int width, height;
		ReadImage(file, data, width, height, 1.f, true);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (data != nullptr)
		{
			delete[] data;
		}
		i++;
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	STextInfo text;
	text.id = tex;
	text.filename = "CubeMap";
	m_Textures.push_back(text);

	return tex;
}

void CTextureLoader::ReloadTexture(string file_name, GLuint* texture_id)
{
	glDeleteTextures(1, texture_id);
	*texture_id = LoadTexture(file_name);
}

void CTextureLoader::ReloadTexture(GLubyte * data, GLuint& texture_id, int width , int height)
{
	glDeleteTextures(1, &texture_id);
	// Create one OpenGL texture
	glGenTextures(1, &texture_id);
	GLenum hubo_error = glGetError();

	if (hubo_error)
	{
		printf("[Error] There was an error reloading the texture :\n");
		return ;
	}
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture_id);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
}

void CTextureLoader::UpdateSubTexture(GLuint & texture_id, GLubyte * subdata, int start_x, int start_y, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, start_x, start_y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)subdata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void CTextureLoader::SaveTextureToFile(string file_name, GLubyte* data, int width, int height)
{
	//FIBITMAP* Image = FreeImage_ConvertFromRawBits(data, width, height, 4, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 32, 8, 8, 8);

	width = FreeImage_GetWidth(bitmap);
	height = FreeImage_GetHeight(bitmap);

	for (int y = height - 1; y >= 0; y--)
	{
		BYTE *bits = FreeImage_GetScanLine(bitmap, height - 1 - y);
		for (int x = 0; x < width; x++)
		{
			bits[0] = data[(y*width + x) * 4 + 2];
			bits[1] = data[(y*width + x) * 4 + 1];
			bits[2] = data[(y*width + x) * 4 + 0];
			bits[3] = data[(y*width + x) * 4 + 3];
			bits += 4;

		}

	}
	FreeImage_FlipVertical(bitmap);
	FreeImage_Save(FIF_PNG, bitmap, file_name.c_str(), PNG_DEFAULT);
	FreeImage_Unload(bitmap);
}


