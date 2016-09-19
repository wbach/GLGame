#include "TerrainRenderer.h"



CTerrainRenderer::CTerrainRenderer(glm::mat4 projection_matrix)
{
	init(projection_matrix);
}

CTerrainRenderer::CTerrainRenderer()
{

}

void CTerrainRenderer::init(glm::mat4 projectionMatrix)
{
	terrainShader.Init(0);
	terrainShader.Start();
	terrainShader.LoadProjectionMatrix(projectionMatrix);
	terrainShader.ConnectTextureUnits();
	terrainShader.Stop();
}

void CTerrainRenderer::render(shared_ptr<CScene>scene,glm::mat4 toShadowSpace)
{
	if (scene->GetTerrains().size() <= 0) return;

	terrainShader.Start();
	terrainShader.LoadIsShadows(0.0f);
	terrainShader.LoadSkyColour(0.6, 0.6, 0.8);
	terrainShader.LoadViewDistance(625);
	terrainShader.LoadViewMatrix(scene->GetViewMatrix());
	terrainShader.LoadClipPlaneVector(glm::vec4(0, 1, 0, 100000));
	terrainShader.LoadLightNumber(scene->GetLights().size());
	terrainShader.LoadToShadowSpaceMatrix(toShadowSpace);


	for (const CTerrain &terrain : scene->GetTerrains())
	{
		prepareTerrain(terrain);
		loadModelMatrix(terrain);
		terrainShader.LoadIsElementOfTerrain(0.0);
		glDrawElements(GL_TRIANGLES, terrain.model.meshes[0].vertexCount, GL_UNSIGNED_INT, 0);
		unBindTextureModel();
	}
	terrainShader.Stop();
}

void CTerrainRenderer::renderElements(CTerrain &terrain){

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
void CTerrainRenderer::prepareTerrain(const CTerrain &terrain)
{
	
	glBindVertexArray(terrain.model.meshes[0].vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	bindTextures(terrain) ;
	// shader.loadShineVariables(0,0);
}

void CTerrainRenderer::bindTextures(const CTerrain &terrain)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, terrain.backgroundTexture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, terrain.rTexture[0]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, terrain.gTexture[0]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, terrain.bTexture[0]);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, terrain.blendMap);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, terrain.backgroundTexture[1]);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, terrain.rTexture[1]);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D, terrain.gTexture[1]);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, terrain.bTexture[1]);

	terrainShader.LoadUseNormalMap(0.0f);
}

void CTerrainRenderer::unBindTextureModel()
{
	Utils::EnableCulling();
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}

void CTerrainRenderer::loadModelMatrix(const CTerrain &terrain)
{
	transformationMatrix = Utils::CreateTransformationMatrix(glm::vec3(terrain.transform.position.x,0 , terrain.transform.position.z),glm::vec3(0),glm::vec3(1));
	terrainShader.LoadTransformMatrix(transformationMatrix);
}
