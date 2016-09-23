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

class CGame;
class CScene
{
public:
	CScene(CGame& game);
	void AddTerrain(CTerrain terrain) { m_Terrains.push_back(terrain); }
	void AddEntity(shared_ptr<CEntity> entity) { m_Entities.push_back(entity); }

	const vector<shared_ptr<CEntity>>& GetEntities() const;
	const vector<CTerrain>& GetTerrains() const;
	const vector<CLight>& GetLights() const;

	const string& GetName();

	virtual int Initialize() = 0;
	virtual int Update() = 0;
	virtual int CleanUp() = 0;
	const virtual glm::mat4& GetViewMatrix() = 0;

	// create position vector (x, heightTerrain(x,z),z)
	glm::vec3 CreatePositionVector(float x, float z, float y_offset = 0);
	glm::vec3 CreatePositionVector(glm::vec2 xzPos, float y_offset = 0);

	//return height form current terrain
	const float GetHeightOfTerrain(float x, float z);
	const float GetHeightOfTerrain(glm::vec2 xz_pos);
	const int TerrainNumber(float x, float z);
	const int TerrainNumber(glm::vec2 xz_pos);

	CLoader& GetLoader() { return m_Loader; }
	const SGUI& GetGui() { return m_Gui; }

	~CScene();
protected:
	CLoader m_Loader;
	string m_Name;
	CGame& m_Game;
	SGUI m_Gui;
	vector<shared_ptr<CEntity>> m_Entities;
	vector<CTerrain> m_Terrains;
	vector<CLight> m_Lights;
	shared_ptr<CCamera> m_Camera;
};



