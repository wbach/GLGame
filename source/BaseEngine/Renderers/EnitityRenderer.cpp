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
				CModel &subModel = scene->GetLoader().m_Models[subEntity->m_ModelId];
				RenderEntity(subEntity, subModel, geomentry_shader);
			}
			CModel &model = scene->GetLoader().m_Models[entity->m_ModelId];
			RenderEntity(entity, model, geomentry_shader);
		}
	}

	for (const shared_ptr<CEntity>& entity : scene->GetEntities())
	{
		for (const shared_ptr<CEntity>& subEntity : entity->GetChildrenEntities())
		{
			CModel &subModel = scene->GetLoader().m_Models[subEntity->m_ModelId];
			RenderEntity(subEntity, subModel, geomentry_shader);
		}
		CModel &model = scene->GetLoader().m_Models[entity->m_ModelId];
		RenderEntity(entity, model, geomentry_shader);
	}
}
void CEntityRenderer::RenderEntity(const shared_ptr<CEntity>& entity, CModel& model, const CEntityGeometryPassShader& geomentry_shader) const
{
	for (CMesh& mesh : model.m_Meshes) 
	{
		glBindVertexArray(mesh.m_Vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		int is_texture = 0, is_normalMap = 0, is_specularMap = 0, is_blendMap = 0; char tmp_name[50] = "";
		int i = 0;
		for (STextInfo& td : mesh.material.textures)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, td.id);
			switch (td.type)
			{
			case MaterialTexture::DIFFUSE: is_texture++;  break;
			case MaterialTexture::NORMAL: is_normalMap++; ; break;
			case MaterialTexture::SPECULAR: is_specularMap++; break;
			case MaterialTexture::BLEND_MAP: is_blendMap ++; break;
			}
			i++;
		}
		
		for (const glm::mat4& mat : entity->GetTransformMatrixes())
		{
			geomentry_shader.LoadTransformMatrix(mat);

			if (mesh.material.isTransparency)
				Utils::DisableCulling();
		
			glDrawElements(GL_TRIANGLES, mesh.m_VertexCount, GL_UNSIGNED_INT, 0);		
		}

		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}