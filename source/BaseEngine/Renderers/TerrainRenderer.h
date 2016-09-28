#pragma once
#include <vector>
#include "../Terrain/Terrain.h"
#include "../Shaders/TerrainGeometryPassShader.h"
#include "../Engine/Scene.h"
class CTerrainRenderer
{	
public:
    void Render(shared_ptr<CScene>scene, const CTerrainGeometryPassShader& geomentry_shader);
    void PrepareTerrain(const CTerrain &terrain);
    void BindTextures(const CTerrain &terrain);
    void UnBindTextureModel();
    void LoadModelMatrix(const CTerrain &terrain, const CTerrainGeometryPassShader& geomentry_shader);
	void RenderElements(CTerrain &terrain);
private:
	glm::mat4 m_TransformationMatrix;
};