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

struct SElement
{
	int value ;
	float flitrDistance;
	vector<glm::vec3> positions;
	glm::vec3 colorOnMap;

	void filtr(){
		vector<glm::vec3> tmp ;
		printf("Size before : %i\n", positions.size());
		vector<glm::vec3>::iterator iter;
		vector<glm::vec3>::iterator it;

		for (it = positions.begin() ; it != positions.end() ; it++)
		{
			bool add = true ;
			for (iter = tmp.begin() ; iter != tmp.end() ; iter++)
			{
				float d = glm::length(*iter - *it);
				if (d < flitrDistance)
				{
					add = false ; break ;
				}
			}
			if(add)
				tmp.push_back(*it);			
		}
		positions = tmp ;
		printf("Size after : %i\n", positions.size());
	}
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

	void AddTerrainEntity(shared_ptr<CEntity> e)
	{ 
		m_TerrainEntities.push_back(e);
	}
	
	void CleanUp();
	void LoadFloora(string flooraMap) ;
	
	void CreateTerrainVertexes(int x_start, int y_start, int width, int height);

	void GenerateTerrainMap(CLoader &loader,string heightMap);
	void CreateTerrain();

	glm::vec3 CalculateNormalMap(int x, int z, FIBITMAP*  image);
	float GetHeightMap(int x, int z, FIBITMAP*  image);

	const float GetHeightofTerrain(glm::vec2 posXZ) const;
	const float GetHeightofTerrain(float worldX, float worldZ) const;
	
	void FiltrElementOffTerrain();
	const float& GetSize() const;
	vector<SElement> GetElementsMap();
	void AddElementToList(glm::vec3 colorOnMap,int value, float floraSize, int i, int j, float distanceFilter);
	SElement* FindElement(int value ,bool &finded);
	void SaveCorrectedFloraMap();

	const int& GetId() { return m_Id; }

	void SetName(std::string name);
	const string GetName() const;
	const string GetNameWithId() const;
	const glm::vec3 GetPosition() const;

	void RecursiveResetEnities(shared_ptr<CEntity>& entity);

	void PaintBlendMap(glm::vec3 point);
	void PaintHeightMap(glm::vec3 point);

	vector<shared_ptr<CEntity>> m_TerrainEntities;
	STransform m_Transform;
	GLuint m_BlendMap;
	GLuint m_BackgroundTexture[2];
	GLuint m_RTexture[2];
	GLuint m_GTexture[2];
	GLuint m_BTexture[2];
	GLuint m_RockTexture[2];

	std::string m_HeightMapPath, m_BlendMapPath, m_BackgorungTexturePath[2], m_RTexturePath[2], m_BTexturePath[2], m_GTexturePath[2], m_RockTexturePath[2];

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

	CLoader&	 m_Loader;
	CEmptyLoader m_Model;
private:
	std::vector<unsigned int> m_Indices;
	std::vector<float> m_Vertices;
	std::vector<float> m_Normals;
	std::vector<float> m_Tangens;
	std::vector<float> m_TextureCoords;


	float m_Size = 1000.0f;
	float m_MaxHeight = 50.0f;	

	glm::vec3 m_Position;

	string m_Name;

	vector<SElement> m_ElementsMap;

	int m_VertexCount;
	float** m_Heights;
	FIBITMAP* m_HighMapImage;

	int m_ImageWidth;
	int m_ImageHeight;

	string m_FlooraMap;
	float m_FloraSize;

	int m_Id;
	static int s_ID;
};
