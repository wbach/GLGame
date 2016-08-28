#include "Scene.h"

vector<shared_ptr<CEntity>> CScene::getEntities() {
	return entities;
}

vector<CTerrain> CScene::getTerrains() { return terrains; }

vector<CLight> CScene::getLights() { return lights; }

string CScene::getName() { return name; }

glm::mat4 CScene::getViewMatrix() { return glm::mat4(1.0); }

// create position vector (x, heightTerrain(x,z),z)

glm::vec3 CScene::createPositionVector(float x, float z, float yOffset) {
	return glm::vec3(x, getHeightOfTerrain(x, z) + yOffset, z);
}

glm::vec3 CScene::createPositionVector(glm::vec2 xzPos, float yOffset) {
	return createPositionVector(xzPos.x, xzPos.y, yOffset);
}

//return height form current terrain
float CScene::getHeightOfTerrain(float x, float z) {
	for (CTerrain ter : terrains) {
		if (x > ter.transform.position.x && x <  ter.transform.position.x + SIZE)
			if (z > ter.transform.position.z && z <  ter.transform.position.z + SIZE) {
				return ter.getHeightofTerrain(x, z);
			}
	}
	return -1;
}

float CScene::getHeightOfTerrain(glm::vec2 xzPos) {
	return getHeightOfTerrain(xzPos.x, xzPos.y);
}

int CScene::terrainNumber(float x, float z) {
	int nr = 0;
	for (CTerrain ter : terrains) {
		if (x > ter.transform.position.x && x <  ter.transform.position.x + SIZE)
			if (z > ter.transform.position.z && z <  ter.transform.position.z + SIZE) {
				return nr;
			}
		nr++;
	}
	return -1;
}

int CScene::terrainNumber(glm::vec2 xzPos)
{
	return terrainNumber(xzPos.x, xzPos.y);
}

CScene::~CScene() {
	loader.cleanUp();
}
