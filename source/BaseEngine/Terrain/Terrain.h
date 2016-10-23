#pragma once
#include "../Entities/Entity.h"
#include "../Loader/Material.h"
#include "../Loader/EmptyLoader.h"
#include "../Loader/Loader.h"
#include "../Engine/Transform.h"
#include "../Utils/Utils.h"
#include <math.h>
#include "glm/glm.hpp"
static const float MAX_PIXEL_COLOUR = 256.0f * 256.0f * 256.0f;
static const int IndicesGridWidth = 500;
static const int IndicesGridHeight = 500;

struct SPaintInfo
{
	glm::vec2 Point;
	float value;
};

class CTerrain
{	
public:	
	CTerrain(CLoader &loader);
	CTerrain(string name, CLoader &loader, 
		float x, float z,
		string height_map, string blend_map,		
		string background_texture, string background_normal_texture, 
		string rock_texture, string rock_normal_texture,
		string r_texture, string r_normal_texture, 
		string g_texture, string g_normal_texture,
		string b_texture, string b_normal_texture);

	void AddTerrainEntity(shared_ptr<CEntity> e);
	vector<int> GetNerbyEntities(glm::vec2 position_xz, int range) const;

	void CleanUp();
	
	void CreateTerrainVertexes(int x_start, int y_start, int width, int height);

	void CreateTerrain();
	void GenerateTerrainMap(CLoader &loader,string heightMap);
	glm::vec3 CalculateNormalMap(int x, int z);
	float GetHeightMap(int x, int z);

	const float GetHeightofTerrain(glm::vec2 posXZ) const;
	const float GetHeightofTerrain(float worldX, float worldZ) const;
	
	const float& GetSize() const;

	const int& GetId() { return m_Id; }

	void SetName(std::string name);
	const string GetName() const;
	const string GetNameWithId() const;
	const glm::vec3 GetPosition() const;

	void RecursiveResetEnities(shared_ptr<CEntity>& entity);

	void PaintBlendMap(glm::vec3 point);
	void PaintHeightMap(glm::vec3 point);
	void PaintHeightMapPoint(glm::vec2 point, BYTE value);
	void ReloadVertex();


	vector<shared_ptr<CEntity>> m_FloraObjects;
	vector<shared_ptr<CEntity>> m_TerrainEntities;
	vector<int>** m_ObjectIndicesGrid;

	STransform m_Transform;
	GLuint m_BlendMap;
	GLuint m_BackgroundTexture[2];
	GLuint m_RTexture[2];
	GLuint m_GTexture[2];
	GLuint m_BTexture[2];
	GLuint m_RockTexture[2];

	std::string m_HeightMapPath, m_BlendMapPath, m_BackgorungTexturePath[2], m_RTexturePath[2], m_BTexturePath[2], m_GTexturePath[2], m_RockTexturePath[2];

	glm::vec3 m_WorldCenterPosition;

	//To painting
	GLubyte* m_BlendMapData = nullptr;
	int m_BlendMapWidth;
	int m_BlendMapHeight;
	glm::vec3 m_PaintColor = glm::vec3(0);
	int m_BrushSize = 50;

	//Terrain height paint
	FIBITMAP* m_HeightMapFreeImage;
	FREE_IMAGE_FORMAT m_HeightMapFormat;
	glm::vec3 m_HeightPaint = glm::vec3(1);
	void SaveHeightMap()const;
	int m_ImageWidth;
	int m_ImageHeight;
	float** m_Heights;

	CLoader&	 m_Loader;
	CEmptyLoader m_Model;
private:
	std::vector<unsigned short> m_Indices;
	std::vector<float> m_Vertices;
	std::vector<float> m_Normals;
	std::vector<float> m_Tangens;
	std::vector<float> m_TextureCoords;


	float m_Size = 100.0f;
	float m_MaxHeight = 50.0f;	

	glm::vec3 m_Position;

	string m_Name;

	int m_VertexCount;
	
	FIBITMAP* m_HighMapImage;	

	string m_FlooraMap;
	float m_FloraSize;

	int m_Id;
	static int s_ID;


};
