#pragma once
#include "../Physics/PhysicsScene.h"
#include "../Controllers/MousePicker.h"
#include "../Entities/Entity.h"
#include "../Loader/Loader.h"
#include "../Lights/Light.h"
#include "../Camera/Camera.h"
#include "../Terrain/Terrain.h"
#include "../GUI/GUI.h"
#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include "../Water/WaterTile.h"
#include <list>
using namespace std;

class CGame;
class CScene
{
public:
	CScene(CGame& game);
	void AddSkyBoxTexture(int type, std::string texture);
	void AddSkyBoxCubeTexture(int type, vector<string> texture);
	void LoadSkyBox();

	void AddLight(CLight light) { m_Lights.push_back(light); }

	void AddEntity(shared_ptr<CEntity>& entity, bool direct = false);
	void AddInstancedEntityFromFile(string file, std::vector<STransform>& transforms, glm::vec3 normalized_size = glm::vec3(0));
	void AddSubEntity(CEntity* parent, shared_ptr<CEntity>& entity);
	
	void CreateNewEmptyTerrain(string name, float x, float z);
	void CreateEmptyHeightMap(string filename, int x, int y);
	void SaveHeightMaps();
	void SaveBlendMaps();
	void MergeTerrains(CTerrain& t1, CTerrain& t2, int axis);
	void MergeAllTerrains();
	CTerrain* FindTerrainByName(string name);

	void ApplyPhysicsToObjects(float dt);
	void ClearObjectsVelocity();

	void DeleteEntity(shared_ptr<CEntity>& entity);
	bool DeleteSubEntity(shared_ptr<CEntity>& entity, int id);
	shared_ptr<CEntity> CreateEntityFromFile(string file_name, bool instanced = false, glm::vec3 pos = glm::vec3(10, 10, 0), glm::vec3 rot = glm::vec3(0), glm::vec3 scale = glm::vec3(1, 1, 1));
	
	shared_ptr<CEntity> FindEntity(int id);
	shared_ptr<CEntity> FindSubEntity(shared_ptr<CEntity>& entity, int id);
	CTerrain* FindTerrainById(int id);

	const vector<shared_ptr<CEntity>>&	GetEntities() const;
	std::vector<CTerrain>&				GetTerrains() ;
	CTerrain*							GetTerrain(int x, int y);
	const vector<CLight>&				GetLights() const;
	vector<CWaterTile>&					GetWaterTiles();

	void LoadTerrainsFloora(string file_name);

	const string& GetName();

	virtual int		Initialize() = 0;
	virtual void	PostInitialize() {};
	virtual int		Update() = 0;
	virtual int		CleanUp() = 0;

	const virtual glm::mat4& GetViewMatrix() = 0;

	// create position vector (x, heightTerrain(x,z),z)
	glm::vec3 CreatePositionVector(float x, float z, float y_offset = 0);
	glm::vec3 CreatePositionVector(glm::vec2 xzPos, float y_offset = 0);

	//return height form current terrain
	const float GetHeightOfTerrain(float x, float z);
	const float GetHeightOfTerrain(glm::vec2 xz_pos);
	void TerrainNumber(float world_x, float world_z, int&x, int& z);
	void TerrainNumber(glm::vec2 xz_pos, int&x, int& z);

	CLoader& GetLoader() { return m_Loader; }
	const SGUI& GetGui() { return m_Gui; }
	
	const glm::vec3			GetCameraPosition() const { return m_Camera->GetPosition(); }
	shared_ptr<CCamera>&	GetCamera() { return m_Camera; }
	
	CGame& GetGameReference() { return m_Game; }

	const glm::vec3 GetDirectionalLightPosition();

	const vector<string>& GetSkyBoxDayTextures() const { return m_DaySkyboxTextures; }
	const vector<string>& GetSkyBoxNightTextures() const { return m_NightSkyboxTextures; }
	CMousePicker& GetMousePicker() { return m_MousePicker; }
	glm::vec3 GetMousePickerTarget();
	void SetEntityToMousePointByKey(std::shared_ptr<CEntity>& entity);
	//Remove only loaded by parser elements

	bool m_IsDebug = false;
	std::string m_SceneFile;

	//ForPainting
	CTerrain*	m_CurrentTerrain;

	CPhysicsScene m_PhysicsScene;
	std::mutex g_pages_mutex;

	std::vector<CTerrain> m_Terrains;
	int m_TerrainsCount;
	int m_TerrainViewRadius;

	float m_HeightPaintStrength = 0.1f;
	bool m_ApplyLimits = false;
	float m_HeightUpperLimit = 100.f;
	float m_HeightDownLimit = 0.f;

	glm::vec3 m_PaintColor = glm::vec3(0);
	int m_BrushSize = 4;

	float m_GloabalTime = 0.f;

	void Reset();
	~CScene();
protected:
	CMousePicker m_MousePicker;
	CLoader m_Loader;
	string	m_Name;
	CGame&	m_Game;
	SGUI	m_Gui;	

	shared_ptr<CCamera> m_Camera;
	vector<CWaterTile>	m_WaterTiles;	
	vector<CLight>		m_Lights;
	vector<CEntity*>	m_PhysicsEntities;
	vector<shared_ptr<CEntity>> m_Entities;
	
	friend class CXmlSceneParser;

	vector<string> m_DaySkyboxTextures;
	vector<string> m_NightSkyboxTextures;
};



