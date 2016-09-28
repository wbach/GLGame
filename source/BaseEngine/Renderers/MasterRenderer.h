#pragma once
#include "../Shaders/EntityGeometryPassShader.h"
#include "../Shaders/TerrainGeometryPassShader.h"
#include "./EnitityRenderer.h"
#include "TerrainRenderer.h"
#include "../Engine/Scene.h"
#include "../Engine/OpenGLRenderer.h"
namespace BufferTexture
{
	enum Type
	{
		POSITION = 0,
		DIFFUSE,
		NORMAL,
		TEXCOORD,
		COUNT
	};
}
class CMasterRenderer
{
public:
	void GeometryPass(shared_ptr<CScene>& scene);
	void LightPass();
	void Init(glm::vec2 window_size, glm::mat4& projection_matrix);
	void CleanUp();
	void DebugRenderTextures();
private:
	int CreateBuffers();
	void SetReadBuffer(BufferTexture::Type TextureType);
	CTerrainRenderer m_TerrainRenderer;
	CEntityRenderer m_EntityRenderer;

	CEntityGeometryPassShader m_EntityGeometryPassShader;
	CTerrainGeometryPassShader m_TerrainGeometryPassShader;

	glm::vec2 m_WindowSize;
	GLuint m_Fbo;
	GLuint m_DepthTexture;
	GLuint m_Textures[BufferTexture::Type::COUNT];
};