#include "TextureLoader.h"

CTextureLoader::CTextureLoader(std::vector<STextInfo>& textures_vector)
	: m_Textures(textures_vector)
{

}

GLuint CTextureLoader::LoadFullTexture(string file_name, bool keepData, GLubyte *&texture, int &width, int &height, bool vertical_flip, float quality)
{
	cout << "Loading texture : " << file_name << endl;

	string file = file_name.substr(file_name.find_last_of('/') + 1);

	for (const STextInfo& t : m_Textures)
	{
		if (file.compare(t.filename) == 0)
			return t.id;
	}
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(file_name.c_str(), 0);

	if (formato == FIF_UNKNOWN) { printf("[Error] %s : wrong image format or file does not exist.\n", file_name.c_str()); return 0; }
	FIBITMAP* imagen2 = FreeImage_Load(formato, file_name.c_str());
	if (!imagen2) { printf("[Error] %s : wrong image format or file does not exist.\n", file_name.c_str()); return 0; }
	FIBITMAP* imagen = FreeImage_ConvertTo32Bits(imagen2);
	FreeImage_Unload(imagen2);

	if (vertical_flip)
		FreeImage_FlipVertical(imagen);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	if (quality < 1)
	{
		w = static_cast<int>(w / quality);
		h = static_cast<int>(h / quality);
		imagen = FreeImage_Rescale(imagen, w, h, FILTER_BILINEAR);
	}

	if (keepData) {
		FreeImage_FlipVertical(imagen);
	}
	char* pixeles = (char*)FreeImage_GetBits(imagen);

	texture = new GLubyte[4 * w*h];

	//bgr2rgb
	for (int j = 0; j<w*h; j++)
	{
		texture[j * 4 + 0] = pixeles[j * 4 + 2];
		texture[j * 4 + 1] = pixeles[j * 4 + 1];
		texture[j * 4 + 2] = pixeles[j * 4 + 0];
		texture[j * 4 + 3] = pixeles[j * 4 + 3];
	}

	if (keepData) {
		width = w;
		height = h;
		FreeImage_Unload(imagen);
		return -1;
	}

	// Create one OpenGL texture
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	GLenum hubo_error = glGetError();
	if (hubo_error)
	{
		printf("[Error] %s : There was an error loading the texture : %s\n", file_name.c_str(), glewGetErrorString(hubo_error));
		delete[] texture;
		return -1;
	}
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture_id);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	delete[] texture;
	FreeImage_Unload(imagen);

	STextInfo text;
	text.id = texture_id;
	text.filename = file;
	m_Textures.push_back(text);
	return texture_id;
}

GLuint CTextureLoader::LoadTexture(string filename, bool verticalFlip)
{
	int w, h; GLubyte* data;
	return LoadFullTexture(filename, false, data, w, h, verticalFlip);
}


