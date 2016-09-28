#include "TerrainRenderer.h"

void CTerrainRenderer::Render(shared_ptr<CScene>scene, const CTerrainGeometryPassShader& geomentry_shader)
{
	if (scene->GetTerrains().size() <= 0) return;

	for (const CTerrain &terrain : scene->GetTerrains())
	{
		PrepareTerrain(terrain);
		LoadModelMatrix(terrain, geomentry_shader);
		glDrawElements(GL_TRIANGLES, terrain.m_Model.m_Meshes[0].m_VertexCount, GL_UNSIGNED_INT, 0);
		UnBindTextureModel();
	}
}

void CTerrainRenderer::RenderElements(CTerrain &terrain)
{


}
void CTerrainRenderer::PrepareTerrain(const CTerrain &terrain)
{
	
	glBindVertexArray(terrain.m_Model.m_Meshes[0].m_Vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	BindTextures(terrain) ;
	// shader.loadShineVariables(0,0);
}

void CTerrainRenderer::BindTextures(const CTerrain &terrain)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrain.m_BackgroundTexture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrain.m_RTexture[0]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrain.m_GTexture[0]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, terrain.m_BTexture[0]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain.m_BlendMap);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, terrain.m_BackgroundTexture[1]);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, terrain.m_RTexture[1]);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, terrain.m_GTexture[1]);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, terrain.m_BTexture[1]);
}

void CTerrainRenderer::UnBindTextureModel()
{
	Utils::EnableCulling();
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void CTerrainRenderer::LoadModelMatrix(const CTerrain &terrain, const CTerrainGeometryPassShader& geomentry_shader)
{
	m_TransformationMatrix = Utils::CreateTransformationMatrix(glm::vec3(terrain.m_Transform.position.x, 0, terrain.m_Transform.position.z), glm::vec3(0), glm::vec3(1));
	geomentry_shader.LoadTransformMatrix(m_TransformationMatrix);
}