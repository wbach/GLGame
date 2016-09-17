#pragma once

static const float SIZE = 2000.0f;
static const float MAX_HEIGHT = 50.0f;
static const float MAX_PIXEL_COLOUR = 256.0f * 256.0f * 256.0f;

//#define VERTEX_COUNT 128
#include "../Entities/Entity.h"
#include "../Loader/Material.h"
#include "../Loader/Model.h"
#include "../Loader/Loader.h"
#include "../Engine/Transform.h"
#include "../Utils/Utils.h"
#include <math.h>
#include "glm/glm.hpp"
#include <SDL2/SDL.h>


struct Element{
	int value ;
	float flitrDistance;
	vector<glm::vec3> positions;


	glm::vec3 colorOnMap;
	void filtr(){
		//positions.clear();return;
		vector<glm::vec3> tmp ;
		//	tmp.push_back(positions[0]);
		printf("Size before : %i\n", positions.size());
		vector<glm::vec3>::iterator iter;
		vector<glm::vec3>::iterator it;

		for (it = positions.begin() ; it != positions.end() ; it++){
			bool add = true ;
			for (iter = tmp.begin() ; iter != tmp.end() ; iter++){
				float d = glm::length(*iter - *it);
				if (d < flitrDistance)
				{
					add = false ; break ;
				}
			}
			if(add){
				tmp.push_back(*it);
			}
		}
		positions = tmp ;
		printf("Size after : %i\n", positions.size());
	}
};

class CTerrain{

	vector<Element> elementsMap ;
	

	int VERTEX_COUNT ;
	float **heights ;
	FIBITMAP* highMapImage ;

	int imageWidth ;
	int imageHeight;

	string flooraMap ;
	float floraSize;
public:
	vector<shared_ptr<CEntity>>terrainEntities;
	STransform transform;
	GLuint blendMap;
	GLuint backgroundTexture[2];
	GLuint rTexture[2];
	GLuint gTexture[2];
	GLuint bTexture[2];
	CModel model;
	CTerrain();
	CTerrain(CLoader &loader,string heightMap, float x, float z, GLuint blendMap, GLuint backgroundTexture, GLuint backgroundNormalTexture,
		GLuint rTexture, GLuint rnormalTexture, GLuint gTexture, GLuint gnormalTexture, GLuint bTexture, GLuint bnormalTexture);

	void addTerrainEntity(shared_ptr<CEntity> e) { terrainEntities.push_back(e); }

	void cleanUp();
	void loadFloora(string flooraMap) ;
	void generateTerrainMap(CLoader &loader,string heightMap);
	glm::vec3 calculateNormalMap(int x, int z, FIBITMAP*  image);
	float getHeightMap(int x, int z, FIBITMAP*  image);
	float getHeightofTerrain(float worldX,float worldZ);
	void filtrElementOffTerrain();
	float getSIZE();
	vector<Element> getElementsMap();
	void addElementToList(glm::vec3 colorOnMap,int value, float floraSize, int i, int j, float distanceFilter);
	Element* findElement(int value ,bool &finded);
	void saveCorrectedFloraMap();
};
