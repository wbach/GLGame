#pragma once
#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"
#include "glm/glm.hpp"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "../../Engine/Scene.h"
#include "../../Camera/ThridPersonCamera.h"
#include "../../Camera/FirstPersonCamera.h"

class CXmlSceneParser
{
public:
	void LoadScene(std::string file_name, CScene* scene, void (*func)(int p)  = nullptr);
	void SaveToFile(std::string file_name, CScene* scene);
	void LoadPrefab(std::string file_name, CScene* scene, rapidxml::xml_node<>* node = nullptr );

	CScene* m_Scene;

 
	int			m_CameraType;
	glm::vec3   m_CameraPosition;
	float		m_CameraPitch;
	float		m_CameraYaw;

private:
	void AddVectorToNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const glm::vec3 vector);
	void AddCubeTextureToNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const vector<string>& textures);
	void AddEntityNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const std::shared_ptr<CEntity>& entity, int global);
	void AddTerrainNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const CTerrain& terrain);
	void AddCameraNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const shared_ptr<CCamera>& camera);
	void AddLightNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const CLight& light);
	void AddSkyBoxNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const vector<string>& day_textures, const vector<string>& night_textures);


	bool ParaseBolean(rapidxml::xml_node<>* node);
	glm::vec3 ParseVector3d(rapidxml::xml_node<>* node);
	std::vector<std::string> ParaseCubeTextures(rapidxml::xml_node<>* node);

	void ParseCamera(rapidxml::xml_node<>* node);
	void ParseSkyBox(rapidxml::xml_node<>* node);
	void ParseLight(rapidxml::xml_node<>* node);
	void ParseTerrain(rapidxml::xml_node<>* node);
	void ParseTexture(rapidxml::xml_node<>* node, std::string& diff_texture, std::string& normal_texture);
	void ParaseEntity(rapidxml::xml_node<>* node, CEntity* parent = nullptr);	
};