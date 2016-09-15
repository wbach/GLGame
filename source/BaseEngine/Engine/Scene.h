#pragma once
#include "../Entities/Entity.h"
#include "../Loader/Loader.h"
#include "../Lights/Light.h"
#include "../Camera/Camera.h"
#include "../Terrain/Terrain.h"
#include "../GUI/GUI.h"
#include <memory>
#include <string>
#include <vector>

using namespace std;


class CScene {
protected:
	string name;
	vector<shared_ptr<CEntity>> entities;

	vector<CLight> lights;

	shared_ptr<CCamera> camera;
public:
	CLoader loader;
	vector<CTerrain> terrains;
	CGUI gui;

	void addTerrain(CTerrain terrain) { terrains.push_back(terrain); }
	void addEntity(shared_ptr<CEntity> entity) { entities.push_back(entity); }

	vector<shared_ptr<CEntity>> getEntities();
	vector<CTerrain> getTerrains();
	vector<CLight> getLights();

	string getName();

	virtual int initialize() = 0;
	virtual int update(SDL_Event &event,SDL_Window *win) = 0;
	virtual int cleanUp() = 0;
	virtual glm::mat4 getViewMatrix();

	// create position vector (x, heightTerrain(x,z),z)
	glm::vec3 createPositionVector(float x, float z, float yOffset = 0);
	glm::vec3 createPositionVector(glm::vec2 xzPos, float yOffset = 0);
	//return height form current terrain
	float getHeightOfTerrain(float x, float z);
	float getHeightOfTerrain(glm::vec2 xzPos);
	int terrainNumber(float x, float z);
	int terrainNumber(glm::vec2 xzPos);
	~CScene();
};



