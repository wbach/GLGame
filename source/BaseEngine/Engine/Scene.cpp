#include "Scene.h"

CScene::CScene(CGame& game) 
: m_Game(game)
{
}

const vector<shared_ptr<CEntity>>& CScene::GetEntities() const
{
	return m_Entities;
}

const vector<CTerrain>& CScene::GetTerrains() const
{ 
	return m_Terrains;
}

const vector<CLight>& CScene::GetLights() const 
{
	return m_Lights;
}

const string& CScene::GetName()
{ 
	return m_Name;
}

glm::vec3 CScene::CreatePositionVector(float x, float z, float y_offset) 
{
	return glm::vec3(x, GetHeightOfTerrain(x, z) + y_offset, z);
}

glm::vec3 CScene::CreatePositionVector(glm::vec2 xzPos, float yOffset) 
{
	return CreatePositionVector(xzPos.x, xzPos.y, yOffset);
}

//return height form current terrain
const float CScene::GetHeightOfTerrain(float x, float z)
{
	for (const CTerrain& ter : m_Terrains) 
		if (x > ter.m_Transform.position.x && x <  ter.m_Transform.position.x + ter.GetSize())
			if (z > ter.m_Transform.position.z && z <  ter.m_Transform.position.z + ter.GetSize()) 
				return ter.GetHeightofTerrain(x, z);			
	
	return -1;
}

const float CScene::GetHeightOfTerrain(glm::vec2 xzPos)
{
	return GetHeightOfTerrain(xzPos.x, xzPos.y);
}

const int CScene::TerrainNumber(float x, float z) 
{
	int nr = 0;
	for (CTerrain ter : m_Terrains) {
		if (x > ter.m_Transform.position.x && x <  ter.m_Transform.position.x + ter.GetSize())
			if (z > ter.m_Transform.position.z && z <  ter.m_Transform.position.z + ter.GetSize())			
				return nr;			
		nr++;
	}
	return -1;
}

const int CScene::TerrainNumber(glm::vec2 xz_pos)
{
	return TerrainNumber(xz_pos.x, xz_pos.y);
}

CScene::~CScene()
{
	m_Loader.CleanUp();
}
