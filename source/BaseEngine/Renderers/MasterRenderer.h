#pragma once
#include "../Shaders/EntityGeometryPassShader.h"
#include "../Shaders/TerrainGeometryPassShader.h"
#include "../Shaders/LightPassShader.h"
#include "./EnitityRenderer.h"
#include "./SkyBoxRenderer.h"
#include "./TerrainRenderer.h"
#include "../Engine/Scene.h"
#include "../Engine/OpenGLRenderer.h"
namespace BufferTexture
{
	enum Type
	{
		POSITION = 0,
		DIFFUSE,
		NORMAL,
		SPECULAR,
		COUNT
	};
}
class CMasterRenderer
{
public:
	void GeometryPass(shared_ptr<CScene>& scene);
	void LightPass(shared_ptr<CScene>& scene);

	void Init(glm::vec2 window_size, glm::mat4& projection_matrix);
	void CleanUp();

	void DebugRenderTextures();

	void SetSkyBoxTextures(GLuint day, GLuint night);
	void SetSkyBoxMeshId(GLuint quad_id, int vertex_count);

	CSkyBoxRenderer& GetSkyBoxRenderer();
private:
	int CreateBuffers();
	void SetReadBuffer(BufferTexture::Type TextureType);
	
	CTerrainRenderer m_TerrainRenderer;
	CEntityRenderer  m_EntityRenderer;
	CSkyBoxRenderer  m_SkyBoxRenderer;

	CEntityGeometryPassShader	m_EntityGeometryPassShader;
	CTerrainGeometryPassShader	m_TerrainGeometryPassShader;
	CLightPassShader			m_LightPassShader;

	glm::vec2	m_WindowSize;
	GLuint		m_Fbo;
	GLuint		m_FinalTexture;
	GLuint		m_DepthTexture;
	GLuint		m_Textures[BufferTexture::Type::COUNT];

	int	   m_QuadIndicesSize;
	GLuint m_QuadVao;
	GLuint m_QuadIndices;
	GLuint m_QuadVertex;
	GLuint m_QuadTexCoord;

	bool m_DebugRenderTextures;
};