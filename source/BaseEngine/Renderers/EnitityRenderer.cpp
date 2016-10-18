#include "EnitityRenderer.h"


void CEntityRenderer::RenderEntityRecursive(const shared_ptr<CScene>& scene, const shared_ptr<CEntity>& entity, const CEntityGeometryPassShader& geomentry_shader)
{
	
	shared_ptr<CModel>& model = scene->GetLoader().m_Models[entity->GetModelId()];
	RenderEntity(entity, *model, geomentry_shader);

	for (const shared_ptr<CEntity>& subEntity : entity->GetChildrenEntities())
	{
		RenderEntityRecursive(scene, subEntity, geomentry_shader);
	}
}

void CEntityRenderer::Render(const shared_ptr<CScene>& scene, const CEntityGeometryPassShader& geomentry_shader)
{
	if (scene->GetEntities().size() <= 0) return;

	

	for (const CTerrain& terr : scene->GetTerrains())
	{		
		for (const shared_ptr<CEntity>& entity : terr.m_TerrainEntities)
		{
			RenderEntityRecursive(scene, entity, geomentry_shader);
		}
	}

	
	for (const shared_ptr<CEntity>& entity : scene->GetEntities())
	{
		RenderEntityRecursive(scene, entity, geomentry_shader);
	}
}
void CEntityRenderer::RenderEntity(const shared_ptr<CEntity>& entity, const CModel& model, const CEntityGeometryPassShader& geomentry_shader)
{
	for (const CMesh& mesh : model.GetMeshes()) 
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
			glDrawElementsInstanced(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, 0, entity->GetTransformMatrixes().size());
		}
		else
		{
			for (const glm::mat4& mat : entity->GetTransformMatrixes())
			{
				geomentry_shader.LoadUseInstancedRendering(0.f);
				geomentry_shader.LoadTransformMatrix(entity->GetRelativeTransformMatrix() * mat);
				glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, 0);
			}
		}
		if (model.UseInstacedRendering())
			glDisableVertexAttribArray(4);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}