#include "MasterRenderer.h"

void CMasterRenderer::Init(shared_ptr<CCamera>& camera, glm::vec2 window_size, glm::mat4& projection_matrix, 
	const float& fov, const float& near, const float& far, float rendering_resolution_modifier, float shadow_map_size, float shadows_distance,
	float water_quality, glm::vec2 reflection_size, glm::vec2 refraction_size,
	float view_distance
	)
{
	m_WindowSize = window_size;

	m_ViewDistance = view_distance + 60;

	m_ShadowMapSize = shadow_map_size;
	m_ShadowsDistance = shadows_distance;

	m_ResoultionMultipler = rendering_resolution_modifier;
	m_DebugRenderTextures = false;

	if (CreateBuffers() == -1)
	{
		std::cout << "[Error] Error in creating master renderer buffers." << std::endl;
	}	

	m_EntityGeometryPassShader.Init();
	m_EntityGeometryPassShader.Start();
	m_EntityGeometryPassShader.LoadProjectionMatrix(projection_matrix);
	m_EntityGeometryPassShader.LoadViewDistance(view_distance + 50);
	m_EntityGeometryPassShader.Stop();

	m_TerrainGeometryPassShader.Init();
	m_TerrainGeometryPassShader.Start();
	m_TerrainGeometryPassShader.LoadProjectionMatrix(projection_matrix);
	m_TerrainGeometryPassShader.LoadViewDistance(view_distance +50);
	m_TerrainGeometryPassShader.Stop();

	m_LightPassShader.Init();
	m_LightPassShader.Start();
	m_LightPassShader.LoadSkyColour(glm::vec3(0.8));
	m_LightPassShader.LoadViewDistance(view_distance);
	m_LightPassShader.LoadProjectionMatrix(projection_matrix);
	m_LightPassShader.Stop();

	Utils::CreateQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord, m_QuadIndicesSize);

	m_SkyBoxRenderer.Init(projection_matrix, far);
	m_ShadowMapRenderer.Init(camera, window_size, fov, near, shadow_map_size, shadows_distance);

	m_ReflectionSize = reflection_size*  m_ResoultionMultipler;
	m_RefractionSize = refraction_size*  m_ResoultionMultipler;
	m_WaterQuality	 = water_quality;

	m_WaterRenderer.Init(window_size, projection_matrix, m_ReflectionSize , m_RefractionSize, m_WaterQuality);

	m_GrassShader.Init();
	m_GrassShader.Start();
	m_GrassShader.LoadProjectionMatrix(projection_matrix);
	m_GrassShader.LoadTransformMatrix(Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(1)));
	m_GrassShader.Stop();

	
}

void CMasterRenderer::CleanUp()
{
	Utils::DeleteQuad(m_QuadVao, m_QuadIndices, m_QuadVertex, m_QuadTexCoord);

	if (m_Textures[0] != 0)
		glDeleteTextures(BufferTexture::Type::COUNT, m_Textures);
	
	if (m_DepthTexture != 0)
		glDeleteTextures(1, &m_DepthTexture);
	
	if (m_Fbo != 0) 
		glDeleteFramebuffers(1, &m_Fbo);	

	m_WaterRenderer.CleanUp();
	m_EntityGeometryPassShader.CleanUp();
	m_TerrainGeometryPassShader.CleanUp();
	m_LightPassShader.CleanUp();
	m_ShadowMapRenderer.CleanUp();
	m_SkyBoxRenderer.CleanUp();
	m_GrassShader.CleanUp();	
}

void CMasterRenderer::SetReadBuffer(BufferTexture::Type TextureType)
{
	glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType);
}

void CMasterRenderer::ShadowPass(shared_ptr<CScene>& scene, const bool& shadows)
{
	if (!shadows)
		return;
	glDepthMask(GL_TRUE);
	m_ShadowMapRenderer.Render(scene);
}
void CMasterRenderer::RenderWaterTextures(shared_ptr<CScene>& scene, const bool& shadows)
{
	glEnable(GL_CLIP_DISTANCE0);
	m_WaterRenderer.SetIsRender(false);

	for (const CWaterTile& tile : scene->GetWaterTiles())
	{		
		glm::vec3 camera_position = scene->GetCamera()->GetPosition();
		float distance = 2 * (camera_position.y - tile.GetPosition().y);
		camera_position.y -= distance;
		scene->GetCamera()->SetPosition(camera_position);
		scene->GetCamera()->InvertPitch();
		scene->GetCamera()->UpdateViewMatrix();
		m_ClipPlane = glm::vec4(0, 1, 0, -tile.GetPosition().y + 0.0f);
		
		m_WaterRenderer.ChangeToReflectionViewPort();
		GeometryPass(scene, shadows);
		LightPass(scene, m_WindowSize, m_WaterRenderer.GetReflectionFrameBuffer());
		m_WaterRenderer.UnbindCurrentFrameBuffer();

		camera_position.y += distance;
		scene->GetCamera()->SetPosition(camera_position);
		scene->GetCamera()->InvertPitch();
		scene->GetCamera()->UpdateViewMatrix();

		m_WaterRenderer.ChangeToRefractionViewPort();
		m_ClipPlane = glm::vec4(0, 1, 0, tile.GetPosition().y - 0.0f);
		GeometryPass(scene, shadows);
		LightPass(scene, m_WindowSize, m_WaterRenderer.GetRefractionFrameBuffer());		

		m_WaterRenderer.UnbindCurrentFrameBuffer();
	}
	m_WaterRenderer.CopyTextureDepthTexture(m_Fbo, m_DepthTexture, m_RefractionSize);

	m_WaterRenderer.SetIsRender(true);
	glDisable(GL_CLIP_DISTANCE0);
}
void CMasterRenderer::Render(shared_ptr<CScene>& scene, const bool& shadows)
{
	if (m_WaterQuality > 0.5f)
	RenderWaterTextures(scene, shadows);

	m_ClipPlane = glm::vec4(0, 1, 0, 100000);

	ShadowPass(scene, shadows);
	glViewport(0, 0, static_cast<int>(m_ResoultionMultipler*m_WindowSize.x), static_cast<int>(m_ResoultionMultipler *m_WindowSize.y));
	GeometryPass(scene, shadows);
	glViewport(0, 0, static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));
	LightPass(scene, m_WindowSize, 0);

}
void CMasterRenderer::GeometryPass(shared_ptr<CScene>& scene, const bool& shadows)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);	
	glDepthMask(GL_TRUE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);

	glDisable(GL_BLEND);
	
	// *********************************************SkyBox render******************************************
	m_SkyBoxRenderer.Render(scene->GetViewMatrix(), 0.f);
	//*****************************************************************************************************
	// ******************************Terrain render********************************************************
	m_TerrainGeometryPassShader.Start();
	if (shadows)
	{
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMapRenderer.GetShadowMap());
		m_TerrainGeometryPassShader.LoadToShadowSpaceMatrix(m_ShadowMapRenderer.GetToShadowMapSpaceMatrix());
	}
	m_TerrainGeometryPassShader.LoadShadowValues(static_cast<float>(shadows), m_ShadowsDistance, m_ShadowMapSize);
	m_TerrainGeometryPassShader.LoadViewMatrix(scene->GetViewMatrix());	
	m_TerrainRenderer.Render(scene, m_TerrainGeometryPassShader);
	m_TerrainGeometryPassShader.Stop();
	//****************************************************************************************************
	// **************************************Entities render**********************************************
	m_EntityGeometryPassShader.Start();
	if (shadows)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMapRenderer.GetShadowMap());
		m_EntityGeometryPassShader.LoadToShadowSpaceMatrix(m_ShadowMapRenderer.GetToShadowMapSpaceMatrix());
	}
	m_EntityGeometryPassShader.LoadClipPlane(m_ClipPlane);
	m_EntityGeometryPassShader.LoadShadowValues(static_cast<float>(shadows),  m_ShadowsDistance, m_ShadowMapSize);
	m_EntityGeometryPassShader.LoadViewMatrix(scene->GetViewMatrix());	
	m_EntityRenderer.Render(scene, m_EntityGeometryPassShader);
	m_EntityGeometryPassShader.Stop();
	//****************************************************************************************************
	// **************************************Grass(Flora) render******************************************
	int x_camera, z_camera, view_radius = scene->m_TerrainViewRadius;
	scene->TerrainNumber(scene->GetCamera()->GetPositionXZ(), x_camera, z_camera);
	
	for (int y = z_camera - view_radius; y < z_camera + view_radius + 1; y++)
		for (int x = x_camera - view_radius; x < x_camera + view_radius + 1; x++)
		{
			if (y < 0 || x < 0 || y > scene->m_TerrainsYCount - 1 || x > scene->m_TerrainsYCount - 1)
				continue;

			CTerrain& terrain = scene->m_Terrains[x][y];
			if (!terrain.m_IsInit) continue;

			m_GrassShader.Start();
			if (shadows)
			{
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, m_ShadowMapRenderer.GetShadowMap());
				m_GrassShader.LoadToShadowSpaceMatrix(m_ShadowMapRenderer.GetToShadowMapSpaceMatrix());
			}
			m_GrassShader.LoadGlobalTime(scene->m_GloabalTime);			
			m_GrassShader.LoadShadowValues(static_cast<float>(shadows), m_ShadowsDistance, m_ShadowMapSize);
			m_GrassShader.LoadViewMatrix(scene->GetViewMatrix());			
			for (CGrass& grass : terrain.m_Grass)
			{
				m_GrassShader.LoadViewDistance(grass.m_ViewDistance);
				grass.Render();
			}
			m_GrassShader.Stop();
		}
	//****************************************************************************************************
	// **************************************Water render******************************************
	m_WaterRenderer.Render(scene, 0.001);

	m_RendererObjectPerFrame = m_EntityRenderer.GetObjectsPerFrame() + m_TerrainRenderer.GetObjectsPerFrame();
	m_RendererVertixesPerFrame = m_EntityRenderer.GetVertexPerFrame() + m_TerrainRenderer.GetVertexPerFrame();

	glDepthMask(GL_FALSE);

	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);	
}

void CMasterRenderer::LightPass(shared_ptr<CScene>& scene, glm::vec2 window_size, GLuint target)
{	
	if (m_DebugRenderTextures)
	{
		DebugRenderTextures();
		return;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target);

	for (unsigned int i = 0; i < BufferTexture::Type::COUNT ; i++) 
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[BufferTexture::Type::POSITION + i]);
	}
	glActiveTexture(GL_TEXTURE0 + BufferTexture::Type::COUNT);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_LightPassShader.Start();
	m_LightPassShader.LoadScreenSize(window_size);
	m_LightPassShader.LoadViewMatrix(scene->GetViewMatrix());
	glm::mat4 transformation_matrix = Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(1.0));
	m_LightPassShader.LoadTransformMatrix(transformation_matrix);
	m_LightPassShader.LoadCameraPosition(scene->GetCameraPosition());
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

	GLint window_w = static_cast<GLint>(m_WindowSize.x);
	GLint window_h = static_cast<GLint>(m_WindowSize.y);

	SetReadBuffer(BufferTexture::Type::POSITION);
	glBlitFramebuffer(0, 0, window_w, window_h, 0, 0, HalfWidth, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	SetReadBuffer(BufferTexture::Type::DIFFUSE);
	glBlitFramebuffer(0, 0, window_w, window_h, 0, HalfHeight, HalfWidth, window_h, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	SetReadBuffer(BufferTexture::Type::NORMAL);
	glBlitFramebuffer(0, 0, window_w, window_h, HalfWidth, HalfHeight, window_w, window_h, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	SetReadBuffer(BufferTexture::Type::SPECULAR);
	glBlitFramebuffer(0, 0, window_w, window_h, HalfWidth, 0, window_w, HalfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
void CMasterRenderer::SetSkyBoxTextures(GLuint day, GLuint night)
{
	m_SkyBoxRenderer.SetTextures(day, night);
}
void CMasterRenderer::SetSkyBoxMeshId(GLuint mesh_id, int vertex_count)
{
	m_SkyBoxRenderer.SetMeshId(mesh_id, vertex_count);
}
const GLuint & CMasterRenderer::GetShadowMap() const
{
	return m_ShadowMapRenderer.GetShadowMap();
}
CSkyBoxRenderer& CMasterRenderer::GetSkyBoxRenderer()
{
	return m_SkyBoxRenderer;
}
int CMasterRenderer::CreateBuffers()
{
	// Create the FBO
	glGenFramebuffers(1, &m_Fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_Fbo);

	// Create the gbuffer textures
	glGenTextures(BufferTexture::Type::COUNT, m_Textures);
	

	for (unsigned int i = 0; i < BufferTexture::Type::COUNT; i++)
	{
		glBindTexture(GL_TEXTURE_2D, m_Textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(m_ResoultionMultipler * m_WindowSize.x), static_cast<int>(m_ResoultionMultipler*m_WindowSize.y), 0, GL_RGBA, GL_FLOAT, NULL);
		if (!m_DebugRenderTextures)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_Textures[i], 0);
	}

	// depth
	glGenTextures(1, &m_DepthTexture);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, static_cast<int>(m_ResoultionMultipler*m_WindowSize.x), static_cast<int>(m_ResoultionMultipler*m_WindowSize.y), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthTexture, 0);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, DrawBuffers);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("[Error]  FB error, status: 0x%x\n", Status);
		return -1;
	}

	// restore default FBO
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return 1;
}
const unsigned int& CMasterRenderer::GetObjectsPerFrame()
{
	return m_RendererObjectPerFrame;
}
const unsigned int& CMasterRenderer::GetVertexPerFrame()
{
	return m_RendererVertixesPerFrame;
}