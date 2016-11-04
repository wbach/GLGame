#include "EnitityRenderer.h"


void CEntityRenderer::RenderEntityRecursive(CScene* scene, CEntity* entity, const CEntityGeometryPassShader& geomentry_shader)
{
	shared_ptr<CModel>& model = scene->GetLoader().m_Models[entity->GetModelId()];
	RenderEntity(entity, *model, geomentry_shader);

	for (const shared_ptr<CEntity>& subEntity : entity->GetChildrenEntities())
	{
		if (subEntity->GetIsCullingChildren() && !subEntity->m_Instanced)
		if (scene->GetCamera()->CheckFrustrumSphereCulling(subEntity->GetWorldPosition(), 1.5f *subEntity->GetMaxNormalizedSize()))
			continue;
		RenderEntityRecursive(scene, subEntity.get(), geomentry_shader);
		m_RendererObjectPerFrame++;
	}
}

void CEntityRenderer::Render(CScene* scene, const CEntityGeometryPassShader& geomentry_shader)
{
	m_RendererObjectPerFrame = 0;
	m_RendererVertixesPerFrame = 0;
	
	for (CTerrain* terrain : scene->GetTerrainsInCameraRange())
	{
		for (const shared_ptr<CEntity>& entity : terrain->m_TerrainEntities)
		{
			if (entity->GetIsCullingChildren() && !entity->m_Instanced)
				if (scene->GetCamera()->CheckFrustrumSphereCulling(entity->GetWorldPosition(), 2.5f * entity->GetMaxNormalizedSize()))
					continue;
			RenderEntityRecursive(scene, entity.get(), geomentry_shader);
			m_RendererObjectPerFrame++;
		}
	}
	
	for (const shared_ptr<CEntity>& entity : scene->GetEntities())
	{
		if (entity->GetIsCullingChildren() && !entity->m_Instanced)
		if (scene->GetCamera()->CheckFrustrumSphereCulling(entity->GetWorldPosition(), 2.5f * entity->GetMaxNormalizedSize()))
			continue;
		RenderEntityRecursive(scene, entity.get(), geomentry_shader);
		m_RendererObjectPerFrame++;
	}

	//cout << "Max vertixec in models on scene: " << m_MaxVerices << endl;
}
void CEntityRenderer::RenderModel(CModel & model, const CEntityGeometryPassShader & geomentry_shader,
	const int& transform_matrixes_size,
	const std::vector<glm::mat4>& transform_matrixes,
	const glm::mat4& relative_matrix
)
{
	geomentry_shader.LoadUseFakeLight(static_cast<float>(model.m_UseFakeLight));


	for (CMesh& mesh : model.GetModifyMeshes())
	{
		glBindVertexArray(mesh.GetVao());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		if (model.UseInstacedRendering())
			glEnableVertexAttribArray(4);

		int is_texture = 0, is_normal_map = 0, is_specular_map = 0, is_blend_map = 0;
		int i = 0;
		for (const STextInfo& td : mesh.GetMaterial().textures)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, td.id);
			switch (td.type)
			{
			case MaterialTexture::DIFFUSE:	 is_texture++;		break;
			case MaterialTexture::NORMAL:	 is_normal_map++;   break;
			case MaterialTexture::SPECULAR:  is_specular_map++; break;
			case MaterialTexture::BLEND_MAP: is_blend_map++;	break;
			}
			i++;
		}
		if (is_normal_map > 0)
			geomentry_shader.LoadUseNormalMap(1.0f);
		else
			geomentry_shader.LoadUseNormalMap(0.0f);

		geomentry_shader.LoadMeshMaterial(mesh.GetMaterial());


		if (mesh.GetMaterial().isTransparency)
			Utils::DisableCulling();

		if (model.UseInstacedRendering())
		{
			geomentry_shader.LoadUseInstancedRendering(1.f);
			glDrawElementsInstanced(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_SHORT, 0, transform_matrixes_size);
		}
		else
		{
			for (const glm::mat4& mat : transform_matrixes)
			{
				geomentry_shader.LoadUseInstancedRendering(0.f);
				geomentry_shader.LoadTransformMatrix(relative_matrix * mat);
				glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_SHORT, 0);
			}
		}
		if (m_MaxVerices < mesh.GetVertexCount())
			m_MaxVerices = mesh.GetVertexCount();
		m_RendererVertixesPerFrame += mesh.GetVertexCount() * transform_matrixes.size();

		glDisable(GL_TEXTURE_2D);
		if (model.UseInstacedRendering())
			glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}
const unsigned int & CEntityRenderer::GetObjectsPerFrame()
{
	return m_RendererObjectPerFrame;
}
const unsigned int & CEntityRenderer::GetVertexPerFrame()
{
	return m_RendererVertixesPerFrame;
}
void CEntityRenderer::RenderEntity(CEntity* entity, CModel& model, const CEntityGeometryPassShader& geomentry_shader)
{
	RenderModel(model, geomentry_shader, entity->GetTransformMatrixes().size() ,entity->GetTransformMatrixes(), entity->GetRelativeTransformMatrix());
}