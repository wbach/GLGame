#include "TerrainRenderer.h"



CTerrainRenderer::CTerrainRenderer(const glm::mat4& projection_matrix)
{
	Init(projection_matrix);
}

CTerrainRenderer::CTerrainRenderer()
{

}

void CTerrainRenderer::Init(const glm::mat4& projection_matrix)
{
	m_TerrainShader.Init(0);
	m_TerrainShader.Start();
	m_TerrainShader.LoadProjectionMatrix(projection_matrix);
	m_TerrainShader.ConnectTextureUnits();
	m_TerrainShader.Stop();
}

void CTerrainRenderer::Render(shared_ptr<CScene>scene, glm::mat4 toShadowSpace)
{
	if (scene->GetTerrains().size() <= 0) return;

	m_TerrainShader.Start();
	m_TerrainShader.LoadIsShadows(0.0f);
	m_TerrainShader.LoadSkyColour(0.6f, 0.6f, 0.8f);
	m_TerrainShader.LoadViewDistance(625.0f);
	m_TerrainShader.LoadViewMatrix(scene->GetViewMatrix());
	m_TerrainShader.LoadClipPlaneVector(glm::vec4(0, 1, 0, 100000));
	m_TerrainShader.LoadLightNumber(static_cast<float>(scene->GetLights().size()));
	m_TerrainShader.LoadToShadowSpaceMatrix(toShadowSpace);


	for (const CTerrain &terrain : scene->GetTerrains())
	{
		PrepareTerrain(terrain);
		LoadModelMatrix(terrain);
		m_TerrainShader.LoadIsElementOfTerrain(0.0);
		glDrawElements(GL_TRIANGLES, terrain.m_Model.m_Meshes[0].m_VertexCount, GL_UNSIGNED_INT, 0);
		UnBindTextureModel();
	}
	m_TerrainShader.Stop();
}

void CTerrainRenderer::RenderElements(CTerrain &terrain){

	/*for ( CMultiPositionMesh mesh : terrain->getElementMeshes()){
		if (!mesh.model->getInstacedRendering()){
			if(mesh.positions.size() <= 0 ) continue;
			glBindVertexArray(mesh.model->getVaoID());
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);
			bool isNormalMap = false;
			for (textureData td : mesh.model->getTextures()){
				if(td.type == DIFFUSE_TEXTURE){		
					glActiveTexture(GL_TEXTURE0 );
					glBindTexture(GL_TEXTURE_2D,td.id) ;
				}else if(td.type == NORMAL_TEXTURE){
					glActiveTexture(GL_TEXTURE5);
					glBindTexture(GL_TEXTURE_2D,td.id) ;
					isNormalMap = true ;
				}
			}
			terrainShader.loadNumberOfRows(mesh.model->getNumberOfRows());
			terrainShader.loadOffset(glm::vec2(mesh.model->getTextureXOffset(),mesh.model->getTextureYOffset()));
			terrainShader.loadIsElementOfTerrain(1.0);
			if (isNormalMap)terrainShader.loadUseNormalMap(1.0f); else	terrainShader.loadUseNormalMap(0.0f);
			if(mesh.model->getUseFakeLighting()) terrainShader.loadUseFakeLight(1.0f);else terrainShader.loadUseFakeLight(0.0f);
			if(mesh.model->getIsTransparency())	 disableCulling();
			for (glm::vec3 position : mesh.positions){
				transformationMatrix = createTransformationMatrix(position, 0,0,0,1);
				terrainShader.loadTransformMatrix(transformationMatrix);
				glDrawElements(GL_TRIANGLES,mesh.model->getVertexCount(),GL_UNSIGNED_INT, 0);
			}
			unBindTextureModel();
		}
		else{

		}
	}*/
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

	m_TerrainShader.LoadUseNormalMap(0.0f);
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

void CTerrainRenderer::LoadModelMatrix(const CTerrain &terrain)
{
	m_TransformationMatrix = Utils::CreateTransformationMatrix(glm::vec3(terrain.m_Transform.position.x, 0, terrain.m_Transform.position.z), glm::vec3(0), glm::vec3(1));
	m_TerrainShader.LoadTransformMatrix(m_TransformationMatrix);
}
