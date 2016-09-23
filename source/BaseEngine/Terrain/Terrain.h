#pragma once
#include "../Entities/Entity.h"
#include "../Loader/Material.h"
#include "../Loader/Model.h"
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
	vector<shared_ptr<CEntity>> m_TerrainEntities;
	STransform m_Transform;
	GLuint m_BlendMap;
	GLuint m_BackgroundTexture[2];
	GLuint m_RTexture[2];
	GLuint m_GTexture[2];
	GLuint m_BTexture[2];
	CModel m_Model;
	CTerrain();
	CTerrain(CLoader &loader,string height_map, float x, float z, GLuint blend_map, GLuint background_texture, GLuint background_normal_texture,
		GLuint r_texture, GLuint r_normal_texture, GLuint g_texture, GLuint g_normal_texture, GLuint b_texture, GLuint b_normal_texture);

	void AddTerrainEntity(shared_ptr<CEntity> e)
	{ 
		m_TerrainEntities.push_back(e);
	}

	void CleanUp();
	void LoadFloora(string flooraMap) ;
	void GenerateTerrainMap(CLoader &loader,string heightMap);
	glm::vec3 CalculateNormalMap(int x, int z, FIBITMAP*  image);
	float GetHeightMap(int x, int z, FIBITMAP*  image);
	const float GetHeightofTerrain(float worldX, float worldZ) const;
	void FiltrElementOffTerrain();
	const float& GetSize() const;
	vector<SElement> GetElementsMap();
	void AddElementToList(glm::vec3 colorOnMap,int value, float floraSize, int i, int j, float distanceFilter);
	SElement* FindElement(int value ,bool &finded);
	void SaveCorrectedFloraMap();

private:
	float m_Size = 2000.0f;
	float m_MaxHeight = 50.0f;	

	vector<SElement> m_ElementsMap;

	int m_VertexCount;
	float** m_Heights;
	FIBITMAP* m_HighMapImage;

	int m_ImageWidth;
	int m_ImageHeight;

	string m_FlooraMap;
	float m_FloraSize;
};
