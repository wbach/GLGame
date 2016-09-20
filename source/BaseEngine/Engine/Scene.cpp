#include "Scene.h"

const vector<shared_ptr<CEntity>>& CScene::GetEntities() const {
	return m_Entities;
}

const vector<CTerrain>& CScene::GetTerrains() const { return m_Terrains; }

const vector<CLight>& CScene::GetLights() const  { return m_Lights; }

const string& CScene::GetName() { return m_Name; }

const glm::mat4& CScene::GetViewMatrix() { return glm::mat4(1.0); }

// create position vector (x, heightTerrain(x,z),z)

glm::vec3 CScene::CreatePositionVector(float x, float z, float y_offset) {
	return glm::vec3(x, GetHeightOfTerrain(x, z) + y_offset, z);
}

glm::vec3 CScene::CreatePositionVector(glm::vec2 xzPos, float yOffset) {
	return CreatePositionVector(xzPos.x, xzPos.y, yOffset);
}

//return height form current terrain
const float CScene::GetHeightOfTerrain(float x, float z) {
	for (CTerrain ter : m_Terrains) {
		if (x > ter.transform.position.x && x <  ter.transform.position.x + SIZE)
			if (z > ter.transform.position.z && z <  ter.transform.position.z + SIZE) {
				return ter.getHeightofTerrain(x, z);
			}
	}
	return -1;
}

const float CScene::GetHeightOfTerrain(glm::vec2 xzPos) {
	return GetHeightOfTerrain(xzPos.x, xzPos.y);
}

const int CScene::TerrainNumber(float x, float z) {
	int nr = 0;
	for (CTerrain ter : m_Terrains) {
		if (x > ter.transform.position.x && x <  ter.transform.position.x + SIZE)
			if (z > ter.transform.position.z && z <  ter.transform.position.z + SIZE) {
				return nr;
			}
		nr++;
	}
	return -1;
}

const int CScene::TerrainNumber(glm::vec2 xzPos)
{
	return TerrainNumber(xzPos.x, xzPos.y);
}

CScene::~CScene() {
	m_Loader.cleanUp();
}
