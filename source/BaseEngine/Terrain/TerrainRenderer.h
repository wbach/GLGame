#ifndef TERRAIN_RENDERER_H
#define TERRAIN_RENDERER_H
#include <vector>
#include "Terrain.h"
#include "TerrainShader.h"
#include "../Engine/Scene.h"
class CTerrainRenderer{
	TerrainShader terrainShader;
	glm::mat4 transformationMatrix;
public:
	CTerrainRenderer();
	CTerrainRenderer(glm::mat4 projectionMatrix);
	void init(glm::mat4 projectionMatrix);
    void render(shared_ptr<CScene>scene,glm::mat4 toShadowSpace);
    void prepareTerrain(CTerrain &terrain);
    void bindTextures(CTerrain &terrain);
    void unBindTextureModel();
    void loadModelMatrix(CTerrain &terrain);
	void renderElements(CTerrain &terrain);
	void cleanUp() { terrainShader.cleanUp(); }
};
#endif