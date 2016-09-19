#include "EnitityRenderer.h"

void CEntityRenderer::Initialize(const glm::mat4& projection_matrix)
{
	m_EntityShader.Init(1);
	m_EntityShader.Start();
	m_EntityShader.LoadProjectionMatrix(projection_matrix);
	m_EntityShader.Stop();

	vector<float> vertex = {-0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0};
	vector<float> text_coords = {
		0,0,
		0,1,
		1,1,
		1,0 };
	vector<unsigned int>indices = {0, 1, 3, 3, 1, 2};

	m_Vao = Utils::CreateVao();
	m_Ivbo = Utils::BindIndicesBuffer(indices);
	m_VboId = Utils::StoreDataInAttributesList(0, 3, vertex);
	m_VboTextId = Utils::StoreDataInAttributesList(1, 2, text_coords);
	Utils::UnbindVao();
}

void CEntityRenderer::Uninitialize()
{
	glDeleteBuffers(1, &m_Ivbo);
	glDeleteBuffers(1, &m_VboId);
	glDeleteBuffers(1, &m_VboTextId);
	glDeleteVertexArrays(1, &m_Vao);
	m_EntityShader.CleanUp();
}

void CEntityRenderer::Render(shared_ptr<CScene>& scene)
{
	if (scene->GetEntities().size() <= 0) return;
	m_EntityShader.Start();

	m_EntityShader.LoadViewMatrix(scene->GetViewMatrix());
	m_EntityShader.LoadIsShadows(0.0f);

	m_EntityShader.LoadLightNumber(scene->GetLights().size());

	m_EntityShader.LoadSkyColour(0.6, 0.6, 0.8);
	m_EntityShader.LoadClipPlaneVector(glm::vec4(0, 1, 0, 100000));
	m_EntityShader.LoadViewDistance(625);

	int nr = 0;
	for (const CLight& light : scene->GetLights())
	{
		m_EntityShader.LoadLight(light, nr++);
	}

	for (const CTerrain& terr : scene->GetTerrains())
	{		
		for (shared_ptr<CEntity> entity : terr.terrainEntities)
		{
			for (shared_ptr<CEntity>& subEntity : entity->GetChildrenEntities())
			{
				CModel &subModel = scene->GetLoader().models[subEntity->m_ModelId];
				RenderEntity(subEntity, subModel);
			}
			CModel &model = scene->GetLoader().models[entity->m_ModelId];
			RenderEntity(entity, model);
		}
	}

	for (shared_ptr<CEntity> entity : scene->GetEntities())
	{
		for (shared_ptr<CEntity> subEntity : entity->GetChildrenEntities())
		{
			CModel &subModel = scene->GetLoader().models[subEntity->m_ModelId];
			RenderEntity(subEntity, subModel);
		}
		CModel &model = scene->GetLoader().models[entity->m_ModelId];
		RenderEntity(entity, model);
	}

	m_EntityShader.Stop();
}
void CEntityRenderer::RenderEntity(shared_ptr<CEntity>& entity, CModel& model)
{
	for (Mesh& mesh : model.meshes) 
	{
		glBindVertexArray(mesh.vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		int is_texture = 0, is_normalMap = 0, is_specularMap = 0, is_blendMap = 0; char tmp_name[50] = "";
		int i = 0;
		for (TextInfo& td : mesh.material.textures)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, td.id);
			switch (td.type)
			{
			case DIFFUSE_TEXTURE: is_texture++;  break;
			case NORMAL_TEXTURE: is_normalMap++; ; break;
			case SPECULAR_TEXTURE: is_specularMap++; break;
			case BLEND_MAP_TEXTURE: is_blendMap ++; break;
			}
			i++;
		}
		if (i == 0)
		{ 
			glBindTexture(GL_TEXTURE_2D, 0);
			m_EntityShader.LoadIsTextured(0.0f);
		}
		else
			m_EntityShader.LoadIsTextured(1.0f);

		if (is_normalMap > 0)
			m_EntityShader.LoadUseNormalMap(1.0f);
		else
			m_EntityShader.LoadUseNormalMap(0.0f);
		
		for (glm::mat4& mat : entity->GetTransformMatrixes())
		{
			m_EntityShader.LoadTransformMatrix(mat);

			if (mesh.material.useFakeLighting)
				m_EntityShader.LoadUseFakeLight(1.0f);
			else
				m_EntityShader.LoadUseFakeLight(0.0f);

			if (mesh.material.isTransparency)
				Utils::DisableCulling();
			
			m_EntityShader.LoadMaterial(mesh.material.diffuse, mesh.material.specular, mesh.material.shineDamper, mesh.material.reflectivity);
			m_EntityShader.LoadNumberOfRows(mesh.material.numberOfRows);
			m_EntityShader.LoadOffset(glm::vec2(mesh.material.getTextureXOffset(), mesh.material.getTextureYOffset()));
		
			glDrawElements(GL_TRIANGLES, mesh.vertexCount, GL_UNSIGNED_INT, 0);		
		}

		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}