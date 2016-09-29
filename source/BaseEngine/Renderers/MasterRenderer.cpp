#include "MasterRenderer.h"

void CMasterRenderer::Init(glm::vec2 window_size, glm::mat4& projection_matrix)
{
	m_WindowSize = window_size;
	if (CreateBuffers() == -1)
	{
		std::cout << "[Error] Error in creating master renderer buffers." << std::endl;
	}
	m_EntityGeometryPassShader.Init();
	m_EntityGeometryPassShader.Start();
	m_EntityGeometryPassShader.LoadProjectionMatrix(projection_matrix);
	m_EntityGeometryPassShader.Stop();

	m_TerrainGeometryPassShader.Init();
	m_TerrainGeometryPassShader.Start();
	m_TerrainGeometryPassShader.LoadProjectionMatrix(projection_matrix);
	m_TerrainGeometryPassShader.Stop();

	m_LightPassShader.Init();
	m_LightPassShader.Start();
	m_LightPassShader.LoadProjectionMatrix(projection_matrix);
	m_LightPassShader.Stop();

	Utils::CreateQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord, m_QuadIndicesSize);

	m_DebugRenderTextures = false;
}

void CMasterRenderer::CleanUp()
{
	if (m_Fbo != 0) 
		glDeleteFramebuffers(1, &m_Fbo);	

	if (m_Textures[0] != 0) 
		glDeleteTextures(BufferTexture::Type::COUNT, m_Textures);	

	if (m_DepthTexture != 0) 
		glDeleteTextures(1, &m_DepthTexture);	

	m_EntityGeometryPassShader.CleanUp();
	m_TerrainGeometryPassShader.CleanUp();
	m_LightPassShader.CleanUp();
	Utils::DeleteQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord);
}

void CMasterRenderer::SetReadBuffer(BufferTexture::Type TextureType)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

void CMasterRenderer::GeometryPass(shared_ptr<CScene>& scene)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);

	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);

	m_TerrainGeometryPassShader.Start();
	m_TerrainGeometryPassShader.LoadViewMatrix(scene->GetViewMatrix());
	m_TerrainRenderer.Render(scene, m_TerrainGeometryPassShader);
	m_TerrainGeometryPassShader.Stop();

	m_EntityGeometryPassShader.Start();
	m_EntityGeometryPassShader.LoadViewMatrix(scene->GetViewMatrix());
	m_EntityRenderer.Render(scene, m_EntityGeometryPassShader);
	m_EntityGeometryPassShader.Stop();

	glDepthMask(GL_FALSE);

	glDisable(GL_DEPTH_TEST);
}

void CMasterRenderer::LightPass(shared_ptr<CScene>& scene)
{	
	if (m_DebugRenderTextures)
	{
		DebugRenderTextures();
		return;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	for (unsigned int i = 0; i < BufferTexture::Type::COUNT ; i++) 
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[BufferTexture::Type::POSITION + i]);
	}

	glClear(GL_COLOR_BUFFER_BIT);

	m_LightPassShader.Start();
	m_LightPassShader.LoadScreenSize(m_WindowSize);
	m_LightPassShader.LoadViewMatrix(scene->GetViewMatrix());
	glm::mat4 transformation_matrix = Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(2.0));
	m_LightPassShader.LoadTransformMatrix(transformation_matrix);
	int lights = scene->GetLights().size();
	m_LightPassShader.LoadLightNumber(lights);
	int i = 0;
	for (const CLight& light : scene->GetLights())
	{
		m_LightPassShader.LoadLight(light, i++);
	}
	Utils::SimpleRenderVao(m_QuadVao, m_QuadIndicesSize);
	m_LightPassShader.Stop();

}
void CMasterRenderer::DebugRenderTextures()
{
	glUseProgram(0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Fbo);

	GLint HalfWidth = (GLint)(m_WindowSize.x / 2.0f);
	GLint HalfHeight = (GLint)(m_WindowSize.y / 2.0f);

	SetReadBuffer(BufferTexture::Type::POSITION);
	glBlitFramebuffer(0, 0, m_WindowSize.x, m_WindowSize.y, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	SetReadBuffer(BufferTexture::Type::DIFFUSE);
	glBlitFramebuffer(0, 0, m_WindowSize.x, m_WindowSize.y, 0, HalfHeight, HalfWidth, m_WindowSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	SetReadBuffer(BufferTexture::Type::NORMAL);
	glBlitFramebuffer(0, 0, m_WindowSize.x, m_WindowSize.y, HalfWidth, HalfHeight, m_WindowSize.x, m_WindowSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	//SetReadBuffer(BufferTexture::Type::TEXCOORD);
	//glBlitFramebuffer(0, 0, m_WindowSize.x, m_WindowSize.y, HalfWidth, 0, m_WindowSize.x, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
int CMasterRenderer::CreateBuffers()
{
	// Create the FBO
	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);

	// Create the gbuffer textures
	glGenTextures(BufferTexture::Type::COUNT, m_Textures);
	glGenTextures(1, &m_DepthTexture);

	for (unsigned int i = 0; i < BufferTexture::Type::COUNT; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_Textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_WindowSize.x, m_WindowSize.y, 0, GL_RGB, GL_FLOAT, NULL);
		if (!m_DebugRenderTextures)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_Textures[i], 0);
	}

	// depth
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_WindowSize.x, m_WindowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		printf("[Error]  FB error, status: 0x%x\n", Status);
		return -1;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return 1;
}