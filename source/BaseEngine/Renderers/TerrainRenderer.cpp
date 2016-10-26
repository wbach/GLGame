#include "TerrainRenderer.h"

void CTerrainRenderer::Render(shared_ptr<CScene> scene, const CTerrainGeometryPassShader& geomentry_shader)
{
	std::vector<std::vector<CTerrain>>& terrains = scene->GetTerrains();
	if (terrains.size() <= 0) return;
	
	m_RendererObjectPerFrame = 0;
	m_RendererVertixesPerFrame = 0;

	int x_camera, z_camera, view_radius = scene->m_TerrainViewRadius;
	scene->TerrainNumber(scene->GetCamera()->GetPositionXZ(), x_camera, z_camera);


	/*for (int y = 0; y < scene->m_TerrainsYCount -1; y++)
		for (int x = 0; x < scene->m_TerrainsXCount - 1; x++)*/		
	for (int y = z_camera - view_radius; y < z_camera + view_radius+1; y++)
		for (int x = x_camera - view_radius; x < x_camera + view_radius+1; x++)
	{
			if (y < 0 || x < 0 || y > scene->m_TerrainsYCount - 1 || x > scene->m_TerrainsYCount - 1)
				continue;

		CTerrain& terrain = terrains[x][y];
		if (!terrain.m_IsInit) continue;

		//if(abs(glm::length(scene->GetCamera()->GetPosition() - terrain.m_WorldCenterPosition)) > 2* terrain.GetSize()) continue;

	//	if (scene->GetCamera()->CheckFrustrumSphereCulling(terrain.m_WorldCenterPosition, terrain.GetSize()))
		//	continue;

		PrepareTerrain(terrain);
		LoadModelMatrix(terrain, geomentry_shader);
		glDrawElements(GL_TRIANGLE_STRIP, terrain.m_Model.GetMeshes()[0].GetVertexCount(), GL_UNSIGNED_SHORT, 0);
		UnBindTextureModel();
		m_RendererObjectPerFrame++;
		m_RendererVertixesPerFrame += terrain.m_Model.GetMeshes()[0].GetVertexCount();
	}
}
const unsigned int& CTerrainRenderer::GetObjectsPerFrame()
{
	return m_RendererObjectPerFrame;
}
const unsigned int& CTerrainRenderer::GetVertexPerFrame()
{
	return m_RendererVertixesPerFrame;
}
void CTerrainRenderer::RenderElements(CTerrain &terrain)
{


}
void CTerrainRenderer::PrepareTerrain(const CTerrain &terrain)
{
	
	glBindVertexArray(terrain.m_Model.GetMeshes()[0].GetVao());
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

	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, terrain.m_RockTexture[0]);
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, terrain.m_RockTexture[1]);
}

void CTerrainRenderer::UnBindTextureModel()
{
	Utils::EnableCulling();
	glDisable(GL_TEXTURE_2D);
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
