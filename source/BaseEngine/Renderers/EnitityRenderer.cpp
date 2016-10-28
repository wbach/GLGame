#include "EnitityRenderer.h"


void CEntityRenderer::RenderEntityRecursive(CScene* scene, CEntity* entity, const CEntityGeometryPassShader& geomentry_shader)
{
	shared_ptr<CModel>& model = scene->GetLoader().m_Models[entity->GetModelId()];
	RenderEntity(scene, entity, *model, geomentry_shader);

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


	if (scene->GetTerrains().size() > 0)
	{
		int x_camera, z_camera, view_radius = scene->m_TerrainViewRadius;
		scene->TerrainNumber(scene->GetCamera()->GetPositionXZ(), x_camera, z_camera);

		for (int y = z_camera - view_radius; y < z_camera + view_radius+1; y++)
			for (int x = x_camera - view_radius; x < x_camera + view_radius+1; x++)
			{
				if (y < 0 || x < 0 || y > scene->m_TerrainsCount || x > scene->m_TerrainsCount)
					continue;		

				CTerrain* terrain = scene->GetTerrain(x, y);
				if (terrain == nullptr)
					continue;

				if (!terrain->m_IsInit) continue;

				if (scene->GetCamera()->CheckFrustrumSphereCulling(terrain->m_WorldCenterPosition, terrain->GetSize() / 1.5f))
					continue;

				for (const shared_ptr<CEntity>& entity : terrain->m_TerrainEntities)
				{
					if (entity->GetIsCullingChildren() && !entity->m_Instanced)
						if (scene->GetCamera()->CheckFrustrumSphereCulling(entity->GetWorldPosition(), 2.5f * entity->GetMaxNormalizedSize()))
							continue;
					RenderEntityRecursive(scene, entity.get(), geomentry_shader);
					m_RendererObjectPerFrame++;
				}
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
const unsigned int & CEntityRenderer::GetObjectsPerFrame()
{
	return m_RendererObjectPerFrame;
}
const unsigned int & CEntityRenderer::GetVertexPerFrame()
{
	return m_RendererVertixesPerFrame;
}
void CEntityRenderer::RenderEntity(CScene* scene, CEntity* entity, CModel& model, const CEntityGeometryPassShader& geomentry_shader)
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
			/*std::vector<glm::mat4> matrixes;
			int i = 0;
			for (const STransform& transform : entity->GetTransforms())
			{
				if (scene->GetCamera()->CheckFrustrumSphereCulling(transform.position, 1.5f * entity->GetMaxNormalizedSize()))
					continue;
				matrixes.push_back(entity->GetTransformMatrixes()[i++]);
			}

			mesh.UpdateTransformVbo(matrixes);
			
			cout << matrixes.size() << endl;*/
			geomentry_shader.LoadUseInstancedRendering(1.f);
			glDrawElementsInstanced(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_SHORT, 0, entity->GetTransformMatrixes().size());
		}
		else
		{
			for (const glm::mat4& mat : entity->GetTransformMatrixes())
			{
				geomentry_shader.LoadUseInstancedRendering(0.f);
				geomentry_shader.LoadTransformMatrix(entity->GetRelativeTransformMatrix() * mat);
				glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_SHORT, 0);
			}
		}
		if (m_MaxVerices < mesh.GetVertexCount())
			m_MaxVerices = mesh.GetVertexCount();
		m_RendererVertixesPerFrame += mesh.GetVertexCount() * entity->GetTransformMatrixes().size();

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