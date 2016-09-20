#pragma once
#include <vector>
#include "Terrain.h"
#include "TerrainShader.h"
#include "../Engine/Scene.h"
class CTerrainRenderer
{	
public:
	CTerrainRenderer();
	CTerrainRenderer(const glm::mat4& projection_matrix);
	void Init(const glm::mat4& projection_matrix);
    void Render(shared_ptr<CScene>scene, glm::mat4 to_shadow_space);
    void PrepareTerrain(const CTerrain &terrain);
    void BindTextures(const CTerrain &terrain);
    void UnBindTextureModel();
    void LoadModelMatrix(const CTerrain &terrain);
	void RenderElements(CTerrain &terrain);
	void CleanUp() { m_TerrainShader.CleanUp(); }
private:
	TerrainShader m_TerrainShader;
	glm::mat4 m_TransformationMatrix;
};