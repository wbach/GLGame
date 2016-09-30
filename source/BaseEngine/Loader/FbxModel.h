#pragma once
#include <string>
#include "Model.h"
#include <GL/glew.h>
#include <fbxsdk.h>
#include "TextureLoader.h"

class CFbxModel : public CModel
{
public:
	CFbxModel(CTextureLoader& texture_lodaer);
	virtual void	InitModel(string file_name) override;
	const string&	GetName() const;
	virtual void	CleanUp() override;
private:
	void InitializeSdkObjects(FbxManager*& manager, FbxScene*& scene);
	bool LoadFile();
	void LoadCacheRecursive(FbxScene* scene, FbxAnimLayer* anim_layer);
	void LoadCacheRecursive(FbxNode* node, FbxAnimLayer* anim_layer);
	void UnloadCacheRecursive(FbxNode* node);
	void UnloadCacheRecursive(FbxScene* scene);
	void DestroySdkObjects(FbxManager* manager, bool exit_status);
	
	bool InitializeMesh(const FbxMesh* mesh);
	bool InitializeMaterial(const FbxSurfaceMaterial* material);

	bool SetCurrentAnimStack(int index);


	void OnTimerClick() const;

	FbxDouble3 GetMaterialProperty(	const FbxSurfaceMaterial*	material,
									const char*					property_name,
									const char*					factor_property_name,
									GLuint&						texture_id);

	std::string		m_FileName;
	std::string		m_FilePath;

	std::vector<SMaterial> m_Materials;

	int				m_PoseIndex;

	FbxManager*		m_SdkManager;
	FbxScene*		m_Scene;
	FbxImporter*	m_Importer;
	FbxAnimLayer*	m_CurrentAnimLayer;
	FbxNode*		m_SelectedNode;

	FbxArray<FbxString*> m_AnimStackNameArray;

	mutable FbxTime m_FrameTime, m_Start, m_Stop, m_CurrentTime;
	mutable FbxTime m_CacheStart, m_CacheStop;

	string			m_Name;
	CTextureLoader&	m_TextureLodaer;
};