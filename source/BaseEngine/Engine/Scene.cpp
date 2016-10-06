#include "Scene.h"

CScene::CScene(CGame& game) 
: m_Game(game)
{
}

void CScene::AddTerrain(CTerrain & terrain) 
{ 
	if (terrain.GetName().size() <=0 || terrain.GetName().compare("No name terrain") == 0)
		terrain.SetName(string("Terrain_") + to_string(m_Terrains.size() ));
	m_Terrains.push_back(terrain); 
}

void CScene::AddEntity(shared_ptr<CEntity> entity, bool direct)
{ 
	if (direct)
	{
		m_Entities.push_back(entity);
		return;
	}
	int tnr = TerrainNumber(entity->GetPositionXZ());

	if (tnr >= 0)
		m_Terrains[tnr].AddTerrainEntity(entity);
	else
		m_Entities.push_back(entity);

}

void CScene::AddSubEntity(shared_ptr<CEntity> parent, shared_ptr<CEntity> entity)
{
	parent->AddSubbEntity(entity);
}

shared_ptr<CEntity> CScene::CreateEntityFromFile(string file_name, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	shared_ptr<CEntity> new_entity;
	new_entity = make_shared<CEntity>(pos, rot, scale);
	new_entity->AddModel(m_Loader.LoadMesh(file_name));

	std::string name = file_name.substr(file_name.find_last_of('\\') + 1);
	if (name.compare(file_name) == 0)
		name = file_name.substr(file_name.find_last_of('/') + 1);
	if (name.compare(file_name) == 0)
		name = "No name";
	new_entity->SetName(name);

	return new_entity;
}

shared_ptr<CEntity> CScene::FindEntity(int id)
{
	shared_ptr<CEntity> founded_entity = nullptr;

	for (CTerrain& terrain : m_Terrains)
	{
		for (shared_ptr<CEntity>& entity : terrain.m_TerrainEntities) 
		{
			if (entity->GetId() == id)
			{
				return entity;
			}
			founded_entity = FindSubEntity(entity,id);
			if (founded_entity != nullptr)
				return founded_entity;
		}		
	}

	for (shared_ptr<CEntity>& entity : m_Entities)
	{
		if (entity->GetId() == id)
		{
			return entity;
		}
		founded_entity = FindSubEntity(entity, id);
		if (founded_entity != nullptr)
			return founded_entity;		
	}

	return founded_entity;
}

shared_ptr<CEntity> CScene::FindSubEntity(shared_ptr<CEntity>& entity, int id)
{
	shared_ptr<CEntity> founded_entity = nullptr;
	for (shared_ptr<CEntity>& entity : entity->GetChildrenEntities())
	{
		if (entity->GetId() == id)
		{
			return entity;
		}
		founded_entity = FindSubEntity(entity, id);
		if (founded_entity != nullptr)
			return founded_entity;
	}
	return nullptr;
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
	for (const CTerrain& ter : m_Terrains) {
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

const glm::vec3 CScene::GetDirectionalLightPosition()
{
	for (const CLight& light : m_Lights)
		if (light.GetType() == LightType::DIRECTIONAL_LIGHT)
			return light.GetPosition();
	cout << "[Error] Directional light not found." << endl;
	return glm::vec3();
}

CScene::~CScene()
{
	m_Loader.CleanUp();
}
