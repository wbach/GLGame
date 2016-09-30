#pragma once
#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h> 
#include "TextureLoader.h"

class CAssimModel  : public CModel
{
public:
	CAssimModel(CTextureLoader& texture_lodaer);
	virtual void	InitModel(string file_name) override;
	const string&	GetName() const;
	virtual void	CleanUp() override;
private:
	void ProcessMesh(string file_path, aiMesh* mesh, const aiScene* scene);
	void RecursiveProcess(string file_path, aiNode *node, const aiScene *scene);

	string			m_Name;	
	CTextureLoader&	m_TextureLodaer;
};