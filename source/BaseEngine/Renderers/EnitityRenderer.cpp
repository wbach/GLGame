#include "EnitityRenderer.h"


void CEntityRenderer::Render(const shared_ptr<CScene>& scene, const  CEntityGeometryPassShader& geomentry_shader) const
{
	if (scene->GetEntities().size() <= 0) return;

	for (const CTerrain& terr : scene->GetTerrains())
	{		
		for (const shared_ptr<CEntity>& entity : terr.m_TerrainEntities)
		{
			for (const shared_ptr<CEntity>& subEntity : entity->GetChildrenEntities())
			{
				shared_ptr<CModel>& subModel = scene->GetLoader().m_Models[subEntity->m_ModelId];
				RenderEntity(subEntity, *subModel, geomentry_shader);
			}
			shared_ptr<CModel>& model = scene->GetLoader().m_Models[entity->m_ModelId];
			RenderEntity(entity, *model, geomentry_shader);
		}
	}

	for (const shared_ptr<CEntity>& entity : scene->GetEntities())
	{
		for (const shared_ptr<CEntity>& subEntity : entity->GetChildrenEntities())
		{
			shared_ptr<CModel>& subModel = scene->GetLoader().m_Models[subEntity->m_ModelId];
			RenderEntity(subEntity, *subModel, geomentry_shader);
		}
		shared_ptr<CModel>& model = scene->GetLoader().m_Models[entity->m_ModelId];
		RenderEntity(entity, *model, geomentry_shader);
	}
}
void CEntityRenderer::RenderEntity(const shared_ptr<CEntity>& entity, CModel& model, const CEntityGeometryPassShader& geomentry_shader) const
{
	for (const CMesh& mesh : model.GetMeshes()) 
	{
		glBindVertexArray(mesh.GetVao());
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

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

		for (const glm::mat4& mat : entity->GetTransformMatrixes())
		{
			geomentry_shader.LoadTransformMatrix(mat);

			if (mesh.GetMaterial().isTransparency)
				Utils::DisableCulling();
		
			glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, 0);		
		}

		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}