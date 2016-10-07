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
	void LoadScene(std::string file_name, CScene* scene);
	void SaveToFile(std::string file_name, CScene* scene);

	CScene* m_Scene;

 
	int			m_CameraType;
	glm::vec3   m_CameraPosition;
	float		m_CameraPitch;
	float		m_CameraYaw;

private:
	void AddVectorToNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const glm::vec3 vector);
	void AddEntityNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const std::shared_ptr<CEntity>& entity, int global);
	void AddTerrainNode(rapidxml::xml_document <>& document, rapidxml::xml_node<>* node, const CTerrain& terrain);

	bool ParaseBolean(rapidxml::xml_node<>* node);
	glm::vec3 ParseVector3d(rapidxml::xml_node<>* node);
	void ParseCamera(rapidxml::xml_node<>* node);
	void ParseTerrain(rapidxml::xml_node<>* node);
	void ParseTexture(rapidxml::xml_node<>* node, std::string& diff_texture, std::string& normal_texture);
	void ParaseEntity(rapidxml::xml_node<>* node, shared_ptr<CEntity> parent = nullptr);	
};