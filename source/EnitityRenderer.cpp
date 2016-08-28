#include "EnitityRenderer.h"



void CEntityRenderer::initialize(glm::mat4 projectionMatrix)
{

	entityShader.init(1);
	entityShader.start();
	entityShader.loadProjectionMatrix(projectionMatrix);
	entityShader.stop();

	vector<float> vertex = { -0.5,0.5,0,-0.5,-0.5,0,0.5,-0.5,0,0.5,0.5,0 };
	vector<float> textCoords = {
		0,0,
		0,1,
		1,1,
		1,0 };
	vector<unsigned int>indices = { 0,1,3,3,1,2 };

	VAO = createVAO();
	ivbo = bindIndicesBuffer(indices);
	vboId = storeDataInAttributesList(0, 3, vertex);
	vboTextId = storeDataInAttributesList(1, 2, textCoords);
	unbindVAO();
}

void CEntityRenderer::uninitialize()
{
	glDeleteBuffers(1, &ivbo);
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &vboTextId);
	glDeleteVertexArrays(1, &VAO);
	entityShader.cleanUp();
}

void CEntityRenderer::render(shared_ptr<CScene>scene)
{
	if (scene->getEntities().size() <= 0)return;
	entityShader.start();

	entityShader.loadViewMatrix(scene->getViewMatrix());
	entityShader.loadIsShadows(0.0f);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture);

	//transformationMatrix *= glm::rotate(-1.0f, 0.0f, 0.0f, 1.0f);


	entityShader.loadLightNumber(scene->getLights().size());

	entityShader.loadSkyColour(0.6, 0.6, 0.8);
	entityShader.loadClipPlaneVector(glm::vec4(0, 1, 0, 100000));
	entityShader.loadViewDistance(625);


	int nr = 0;
	for (CLight light : scene->getLights()) {
		entityShader.loadLight(light, nr++);
	}

	for (CTerrain terr : scene->terrains) {		
		for (shared_ptr<CEntity> entity : terr.terrainEntities) {

			for (shared_ptr<CEntity> subEntity : entity->entities) {
				CModel &subModel = scene->loader.models[subEntity->model_id];
				renderEntity(subEntity, subModel);
			}
			CModel &model = scene->loader.models[entity->model_id];
			renderEntity(entity, model);
		}
	}


	for (shared_ptr<CEntity> entity : scene->getEntities()) {

		for (shared_ptr<CEntity> subEntity : entity->entities) {
			CModel &subModel = scene->loader.models[subEntity->model_id];
			renderEntity(subEntity, subModel);
		}
		CModel &model = scene->loader.models[entity->model_id];
		renderEntity(entity, model);
	}

	/*glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);*/

	entityShader.stop();
}
void CEntityRenderer::renderEntity(shared_ptr<CEntity> entity, CModel &model)
{
	

	for (Mesh mesh : model.meshes) {

		glBindVertexArray(mesh.vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		int isTexture = 0, isNormalMap = 0, isSpecularMap = 0, isBlendMap = 0; char tmpName[50] = "";
		int i = 0;
		for (TextInfo td : mesh.material.textures) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, td.id);
			switch (td.type) {
			case DIFFUSE_TEXTURE: isTexture++;  break;
			case NORMAL_TEXTURE: isNormalMap++; ; break;
			case SPECULAR_TEXTURE: isSpecularMap++; break;
			case BLEND_MAP_TEXTURE: isBlendMap++; break;
			}
			i++;
		}
		if (i == 0) { glBindTexture(GL_TEXTURE_2D, 0); entityShader.loadIsTextured(0.0f); }
		else entityShader.loadIsTextured(1.0f);

		if (isNormalMap > 0) entityShader.loadUseNormalMap(1.0f); else	entityShader.loadUseNormalMap(0.0f);	
		
		for (glm::mat4 mat : entity->getTransformMatrixes()) {		

			entityShader.loadTransformMatrix(mat);

			if (mesh.material.useFakeLighting)	entityShader.loadUseFakeLight(1.0f); else entityShader.loadUseFakeLight(0.0f);
			if (mesh.material.isTransparency) {
				disableCulling();
			}
			entityShader.loadMaterial(mesh.material.diffuse, mesh.material.specular, mesh.material.shineDamper, mesh.material.reflectivity);
			entityShader.loadNumberOfRows(mesh.material.numberOfRows);
			entityShader.loadOffset(glm::vec2(mesh.material.getTextureXOffset(), mesh.material.getTextureYOffset()));
		
			glDrawElements(GL_TRIANGLES, mesh.vertexCount, GL_UNSIGNED_INT, 0);
		
		}


		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}
}