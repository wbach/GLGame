#include "Scene.h"
#include "GLgame.h"
CScene::CScene(CGame& game) 
: m_Game(game)
{
	m_Loader.SetMaxTextureResolution(m_Game.GetMaxTextureResolution());
}

void CScene::AddSkyBoxTexture(int type, std::string texture)
{
	if (type == 0)
		m_DaySkyboxTextures.push_back(texture);
	else
		m_NightSkyboxTextures.push_back(texture);
}

void CScene::AddSkyBoxCubeTexture(int type, vector<string> texture)
{
	if (type == 0)
		m_DaySkyboxTextures = texture;
	else
		m_NightSkyboxTextures = texture;
}

void CScene::LoadSkyBox()
{
	if (m_DaySkyboxTextures.size() == 6 && m_NightSkyboxTextures.size() == 6)
	{
		GLuint day_texture = m_Loader.LoadCubeMap(m_DaySkyboxTextures);
		GLuint night_texture = m_Loader.LoadCubeMap(m_NightSkyboxTextures);

		m_Game.GetMasterRenderer().SetSkyBoxTextures(day_texture, night_texture);
		int skybox_cube = m_Loader.LoadMesh("Data/Meshes/SkyBox/cube.obj");
		if (skybox_cube >= 0)
		m_Game.GetMasterRenderer().SetSkyBoxMeshId(m_Loader.m_Models[skybox_cube]->GetMeshes()[0].GetVao(), m_Loader.m_Models[skybox_cube]->GetMeshes()[0].GetVertexCount());
	}
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

void CScene::SaveTerrains()
{
	for (const CTerrain& terrain : m_Terrains)
	{
		m_Loader.SaveTextureToFile(terrain.m_BlendMapPath, terrain.m_BlendMapData, terrain.m_BlendMapWidth, terrain.m_BlendMapHeight);
		terrain.SaveHeightMap();
	}
}

void CScene::ApplyPhysicsToObjects(float dt)
{
	int x = 0;
	for (shared_ptr<CEntity>& entity : m_PhysicsEntities)
	{
		entity->IncrasePosition(entity->m_RigidBody.m_AngularVelocity *dt );

		float h = 0;
		x++;
		h = GetHeightOfTerrain(entity->GetPositionXZ());

		if (entity->GetWorldPosition().y < h)
		{
			entity->SetPositionY(h);
			entity->m_RigidBody.ReflectVelocity(glm::vec3(0, 1, 0));
			entity->SetIsInAir(false);
		}
		entity->m_RigidBody.SetPosition(entity->GetWorldPosition());
	}
}

void CScene::ClearObjectsVelocity()
{
	for (shared_ptr<CEntity> entity :m_PhysicsEntities)
	{
		float h = GetHeightOfTerrain(entity->GetPositionXZ());
		entity->SetPositionY(h);
		entity->m_RigidBody.m_AngularVelocity = glm::vec3(0, 0, 0);
		entity->m_RigidBody.SetPosition(entity->GetWorldPosition() + glm::vec3(0,50,0));
	}
}

void CScene::DeleteEntity(shared_ptr<CEntity> deleted_entity)
{
	bool deleted = false;
	for (CTerrain& terrain : m_Terrains)
	{
		int i = 0;
		for (shared_ptr<CEntity>& entity : terrain.m_TerrainEntities)
		{
			if (entity->GetId() == deleted_entity->GetId() )
			{
				entity->CleanUp();
				terrain.m_TerrainEntities.erase(terrain.m_TerrainEntities.begin() + i);
				return;
			}
			deleted = DeleteSubEntity(entity, deleted_entity->GetId());
			if (deleted)
				return;
			i++;
		}
	}
	int i = 0;
	for (shared_ptr<CEntity>& entity : m_Entities)
	{
		if (entity->GetId() == deleted_entity->GetId())
		{
			entity->CleanUp();
			m_Entities.erase(m_Entities.begin() + i);
			return;
		}
		deleted = DeleteSubEntity(entity, deleted_entity->GetId());
		if (deleted)
			return;
		i++;
	}
}
bool CScene::DeleteSubEntity(shared_ptr<CEntity>& entity, int id)
{
	bool deleted = false;
	int i = 0;
	for (shared_ptr<CEntity>& subentity : entity->GetChildrenEntities())
	{
		if (subentity->GetId() == id)
		{
			subentity->CleanUp();
			entity->GetChildrenEntities().erase(entity->GetChildrenEntities().begin() + i);
			return true;
		}
		deleted = DeleteSubEntity(subentity, id);
		if (deleted)
			return true;
		i++;
	}
	return false;
}
shared_ptr<CEntity> CScene::CreateEntityFromFile(string file_name, bool instanced, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	
	shared_ptr<CEntity> new_entity;
	new_entity = make_shared<CEntity>(pos, rot, scale);	

	int model_id = m_Loader.LoadMesh(file_name);

	if (model_id == -1) return nullptr;

	new_entity->m_BoundingSize = m_Loader.m_Models[model_id]->GetBoundingMaxSize();
	new_entity->AddModel(model_id);
	if(instanced)
		m_Loader.m_Models[model_id]->CreateTransformsVbo(new_entity->GetTransformMatrixes());

	std::string name = file_name.substr(file_name.find_last_of('\\') + 1);
	if (name.compare(file_name) == 0)
		name = file_name.substr(file_name.find_last_of('/') + 1);
	if (name.compare(file_name) == 0)
		name = "No name";
	new_entity->SetName(name);
	new_entity->SetFullPath(file_name);
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

CTerrain * CScene::FindTerrainById(int id)
{
	for (CTerrain& terrain : m_Terrains)
	{
		if (terrain.GetId() == id)
		{
			return &terrain;
		}
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
	//cout << "[Error] Directional light not found." << endl;
	throw std::runtime_error(std::string("Directional light not found.").c_str());;
	return glm::vec3();
}

glm::vec3 CScene::GetMousePickerTarget()
{
	for (CTerrain& terrain : m_Terrains)
	{  
		bool is_col;
		glm::vec3 point = m_MousePicker.GetMousePointOnTerrain(m_Game.GetInputManager().GetMousePosition(), terrain, is_col);
		if (is_col) return point;
	}
	return glm::vec3();
}

void CScene::SetEntityToMousePointByKey(std::shared_ptr<CEntity> entity)
{
	if (!m_Game.GetInputManager().GetKey(KeyCodes::M)) return;
	for (CTerrain& terrain : m_Terrains)
	{
		bool is_col;
		glm::vec3 point = m_MousePicker.GetMousePointOnTerrain(m_Game.GetInputManager().GetMousePosition(), terrain, is_col);
		if (is_col)
		{
			entity->SetPosition(CreatePositionVector(point.x, point.z, entity->GetAttachYOffset()));
			return;
		}
	}
}

void CScene::Reset()
{
	CleanUp();
	Initialize();
}

CScene::~CScene()
{
	m_Loader.CleanUp();
}
