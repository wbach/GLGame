#pragma once
#include "../Shaders/EntityGeometryPassShader.h"
#include "../Shaders/TerrainGeometryPassShader.h"
#include "../Shaders/LightPassShader.h"
#include "./EnitityRenderer.h"
#include "./SkyBoxRenderer.h"
#include "../Shadows/ShadowMapRenderer.h"
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
	void ShadowPass(shared_ptr<CScene>& scene, const bool& shadows);
	void GeometryPass(shared_ptr<CScene>& scene, const bool& shadows);
	void LightPass(shared_ptr<CScene>& scene);

	void Init(shared_ptr<CCamera>& camera, glm::vec2 window_size, glm::mat4& projection_matrix, float shadow_map_size = 2048);
	void CleanUp();

	void DebugRenderTextures();

	void SetSkyBoxTextures(GLuint day, GLuint night);
	void SetSkyBoxMeshId(GLuint quad_id, int vertex_count);
	const GLuint& GetShadowMap() const;
	CSkyBoxRenderer& GetSkyBoxRenderer();

	const unsigned int& GetObjectsPerFrame();
	const unsigned int& GetVertexPerFrame();
private:
	int CreateBuffers();
	void SetReadBuffer(BufferTexture::Type TextureType);

	CTerrainRenderer   m_TerrainRenderer;
	CEntityRenderer    m_EntityRenderer;
	CSkyBoxRenderer    m_SkyBoxRenderer;
	CShadowMapRenderer m_ShadowMapRenderer;

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

	int m_ResoultionMultipler = 1;

	bool m_DebugRenderTextures;

	unsigned int m_RendererObjectPerFrame = 0;
	unsigned int m_RendererVertixesPerFrame = 0;
};