#include "FbxModel.h"

CFbxModel::CFbxModel(CTextureLoader & texture_lodaer)
: m_TextureLodaer(texture_lodaer)
, m_SdkManager(nullptr)
, m_Scene(nullptr)
, m_Importer(nullptr)
, m_CurrentAnimLayer(nullptr)
, m_SelectedNode(nullptr)
, m_PoseIndex(-1)
{
}

void CFbxModel::InitModel(string file_name)
{
	m_FileName = file_name;
	m_FilePath = file_name.substr(0, file_name.find_last_of('/')) + "/";

	// initialize cache start and stop time
	m_CacheStart	= FBXSDK_TIME_INFINITE;
	m_CacheStop	= FBXSDK_TIME_MINUS_INFINITE;

	// Create the FBX SDK manager which is the object allocator for almost 
	// all the classes in the SDK and create the scene.
	InitializeSdkObjects(m_SdkManager, m_Scene);

	if (m_SdkManager)
	{
		// Create the importer.
		int file_format = -1;
		m_Importer = FbxImporter::Create(m_SdkManager, "");
		if (!m_SdkManager->GetIOPluginRegistry()->DetectReaderFileFormat(m_FileName.c_str(), file_format))
		{
			// Unrecognizable file format. Try to fall back to FbxImporter::eFBX_BINARY
			file_format = m_SdkManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");;
		}

		// Initialize the importer by providing a filename.
		if (m_Importer->Initialize(m_FileName.c_str(), file_format) == false)
		{
			std::cout << "[Error] Cant init model : " << m_FileName;
		}		
	}
	LoadFile();
}
void CFbxModel::InitializeSdkObjects(FbxManager*& manager, FbxScene*& scene)
{

	//The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
	manager = FbxManager::Create();
	if (!manager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		exit(1);
	}
	//else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	manager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString path = FbxGetApplicationDirectory();
	manager->LoadPluginsDirectory(path.Buffer());

	//Create an FBX scene. This object holds most objects imported/exported from/to files.
	scene = FbxScene::Create(manager, "My Scene");
	if (!scene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		exit(1);
	}
}
bool CFbxModel::LoadFile()
{
	bool result = false;
	// Make sure that the scene is ready to load.

	if (m_Importer->Import(m_Scene) == true)
	{
		// Set the scene status flag to refresh 
		// the scene in the first timer callback.

		// Convert Axis System to what is used in this example, if needed
		FbxAxisSystem scene_axis_system = m_Scene->GetGlobalSettings().GetAxisSystem();
		FbxAxisSystem our_axis_system(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
		if (scene_axis_system != our_axis_system)
		{
			our_axis_system.ConvertScene(m_Scene);
		}

		// Convert Unit System to what is used in this example, if needed
		FbxSystemUnit scene_system_unit = m_Scene->GetGlobalSettings().GetSystemUnit();
		if (scene_system_unit.GetScaleFactor() != 1.0)
		{
			//The unit in this example is centimeter.
			FbxSystemUnit::cm.ConvertScene(m_Scene);
		}

		// Get the list of all the animation stack.
		m_Scene->FillAnimStackNameArray(m_AnimStackNameArray);

		// Get the list of all the cameras in the scene.
		// FillCameraArray(mScene, mCameraArray);

		// Convert mesh, NURBS and patch into triangle mesh
		FbxGeometryConverter geom_converter(m_SdkManager);

		geom_converter.Triangulate(m_Scene, true, true);//node //<<------------------------------------------timing more..

													  // Split meshes per material, so that we only have one material per mesh (for VBO support)
		geom_converter.SplitMeshesPerMaterial(m_Scene, /*replace*/true);

		// Bake the scene for one frame
		LoadCacheRecursive(m_Scene, m_CurrentAnimLayer);

		// Initialize the frame period.
		m_FrameTime.SetTime(0, 0, 0, 1, 0, m_Scene->GetGlobalSettings().GetTimeMode());

		result = true;
	}
	else
	{
		// Import failed, set the scene status flag accordingly.
		std::cout << "Unable to import file " << m_FileName << endl;
	}

	// Destroy the importer to release the file.
	m_Importer->Destroy();
	m_Importer = nullptr;

	return result;
}
void CFbxModel::LoadCacheRecursive(FbxScene* scene, FbxAnimLayer* anim_layer)
{
	// Load the textures into GPU, only for file texture now
	const int texture_count = scene->GetTextureCount();
	for (int texture_index = 0; texture_index < texture_count; ++texture_index)
	{
		FbxTexture* texture = scene->GetTexture(texture_index);
		FbxFileTexture* file_texture = FbxCast<FbxFileTexture>(texture);
		if (file_texture && !file_texture->GetUserDataPtr())
		{
			// Try to load the texture from absolute path
			const FbxString file_name = file_texture->GetFileName();
			FBXSDK_printf("%s\n", file_name.Buffer());

			if (file_name.Right(3).Upper() == "TGA")
			{
				FBXSDK_printf("TGA textures are not supported now: %s\n", file_name.Buffer());
				continue;
			}

			GLuint texture_id = 0;
			std::string texture_file(file_name.Buffer());
			texture_file = m_FilePath + texture_file.substr(texture_file.find_last_of("/") + 1);

			texture_id = m_TextureLodaer.LoadTexture(texture_file); //<<---texture		

			if (!texture_id)
			{
				FBXSDK_printf("Failed to load texture file: %s\n", file_name.Buffer());
				continue;
			}

			if (texture_id)
			{
				GLuint * lTextureName = new GLuint(texture_id);
				file_texture->SetUserDataPtr(lTextureName);
			}
		}
	}

	LoadCacheRecursive(scene->GetRootNode(), anim_layer);
}
void CFbxModel::LoadCacheRecursive(FbxNode* node, FbxAnimLayer* anim_layer)
{
	// Bake material and hook as user data.
	const int material_count = node->GetMaterialCount();
	for (int material_index = 0; material_index < material_count; ++material_index)
	{
		FbxSurfaceMaterial * material = node->GetMaterial(material_index);
		if (material && !material->GetUserDataPtr())
		{		
			InitializeMaterial(material);			
		}
	}

	FbxNodeAttribute* node_attribute = node->GetNodeAttribute();
	if (node_attribute)
	{
		// Bake mesh as VBO(vertex buffer object) into GPU.
		if (node_attribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh* mesh = node->GetMesh();
			if (mesh && !mesh->GetUserDataPtr())
			{				
				InitializeMesh(mesh);				
			}
		}
	}

	const int lChildCount = node->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		LoadCacheRecursive(node->GetChild(lChildIndex), anim_layer);
	}

}
bool CFbxModel::InitializeMesh(const FbxMesh * pMesh)
{
	if (!pMesh->GetNode())
		return false;
	SMaterial material;

	vector<float> postions;
	vector<float> text_coords;
	vector<float> normals;
	vector<float> tangents;
	vector<float> diffuse;
	vector<float> specular;
	vector<float> ambient;
	vector<unsigned int> indices;

	const int lPolygonCount = pMesh->GetPolygonCount();

	// Count the polygon count of each material
	FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
	FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;
	if (pMesh->GetElementMaterial())
	{
		lMaterialIndice = &pMesh->GetElementMaterial()->GetIndexArray();
		lMaterialMappingMode = pMesh->GetElementMaterial()->GetMappingMode();		
	}	

	FbxStringList uv_names;
	pMesh->GetUVSetNames(uv_names);
	const char* uv_name = nullptr;
	if (uv_names.GetCount())
	{		
		uv_name = uv_names[0];
	}

	// Populate the array with vertex attribute, if by control point.
	const FbxVector4 * lControlPoints = pMesh->GetControlPoints();
	FbxVector4 current_vertex;
	FbxVector4 lCurrentNormal;
	FbxVector2 lCurrentUV;
	
	int lVertexCount = 0;
	for (int lPolygonIndex = 0; lPolygonIndex < lPolygonCount; ++lPolygonIndex)
	{
		// The material for current face.
		int lMaterialIndex = 0;
		if (lMaterialIndice)// && lMaterialMappingMode == FbxGeometryElement::eByPolygon)
		{			
			lMaterialIndex = lMaterialIndice->GetAt(lPolygonIndex);
			material = m_Materials[lMaterialIndex];
			//material.diffuse = glm::vec3(1.0);
		}

		for (int lVerticeIndex = 0; lVerticeIndex < 3; ++lVerticeIndex)
		{
			const int lControlPointIndex = pMesh->GetPolygonVertex(lPolygonIndex, lVerticeIndex);
			
			indices.push_back(static_cast<unsigned int>(lVertexCount));

			current_vertex = lControlPoints[lControlPointIndex];
			postions.push_back(static_cast<float>(current_vertex[0]));
			postions.push_back(static_cast<float>(current_vertex[1]));
			postions.push_back(static_cast<float>(current_vertex[2]));

			tangents.push_back(0);
			tangents.push_back(0);
			tangents.push_back(0);

			pMesh->GetPolygonVertexNormal(lPolygonIndex, lVerticeIndex, lCurrentNormal);
			normals.push_back(static_cast<float>(lCurrentNormal[0]));
			normals.push_back(static_cast<float>(lCurrentNormal[1]));
			normals.push_back(static_cast<float>(lCurrentNormal[2]));

			bool lUnmappedUV;
			pMesh->GetPolygonVertexUV(lPolygonIndex, lVerticeIndex, uv_name, lCurrentUV, lUnmappedUV);
			text_coords.push_back(static_cast<float>(lCurrentUV[0]));
			text_coords.push_back(static_cast<float>(lCurrentUV[1]));			
			
			++lVertexCount;
		}
	}
	AddMesh("FBX mesh", postions, text_coords, normals, tangents, indices, material);
	return false;
}

bool CFbxModel::InitializeMaterial(const FbxSurfaceMaterial * pMaterial)
{
	SMaterial material;
	GLuint ambient_textur = 0;
	const FbxDouble3 ambient = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, ambient_textur);
	material.ambient.x = static_cast<GLfloat>(ambient[0]);
	material.ambient.y = static_cast<GLfloat>(ambient[1]);
	material.ambient.z = static_cast<GLfloat>(ambient[2]);

	if (ambient_textur > 0)
	{
		STextInfo texture_info;
		texture_info.id = ambient_textur;
		texture_info.type = MaterialTexture::AMBIENT;
		material.textures.push_back(texture_info);
	}

	GLuint diffuse_textur = 0;
	const FbxDouble3 diffuse = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, diffuse_textur);
	material.diffuse.x = static_cast<GLfloat>(diffuse[0]);
	material.diffuse.y = static_cast<GLfloat>(diffuse[1]);
	material.diffuse.z = static_cast<GLfloat>(diffuse[2]);

	if (diffuse_textur > 0)
	{
		STextInfo texture_info;
		texture_info.id = diffuse_textur;
		texture_info.type = MaterialTexture::DIFFUSE;
		material.textures.push_back(texture_info);
	}

	GLuint specular_textur = 0;
	const FbxDouble3 specular = GetMaterialProperty(pMaterial,
		FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, specular_textur);
	material.specular.x = static_cast<GLfloat>(specular[0]);
	material.specular.y = static_cast<GLfloat>(specular[1]);
	material.specular.z = static_cast<GLfloat>(specular[2]);

	if (specular_textur > 0)
	{
		STextInfo texture_info;
		texture_info.id = specular_textur;
		texture_info.type = MaterialTexture::SPECULAR;
		material.textures.push_back(texture_info);
	}

	FbxProperty shininess_property = pMaterial->FindProperty(FbxSurfaceMaterial::sShininess);
	if (shininess_property.IsValid())
	{
		double shininess = shininess_property.Get<FbxDouble>();
		material.shineDamper = static_cast<GLfloat>(shininess);
	}
	m_Materials.push_back(material);
	return true;
}

bool CFbxModel::SetCurrentAnimStack(int index)
{
	const int lAnimStackCount = m_AnimStackNameArray.GetCount();
	if (!lAnimStackCount || index >= lAnimStackCount)
	{
		return false;
	}

	// select the base layer from the animation stack
	FbxAnimStack * lCurrentAnimationStack = m_Scene->FindMember<FbxAnimStack>(m_AnimStackNameArray[index]->Buffer());
	if (lCurrentAnimationStack == NULL)
	{
		// this is a problem. The anim stack should be found in the scene!
		return false;
	}

	// we assume that the first animation layer connected to the animation stack is the base layer
	// (this is the assumption made in the FBXSDK)
	m_CurrentAnimLayer = lCurrentAnimationStack->GetMember<FbxAnimLayer>();
	m_Scene->SetCurrentAnimationStack(lCurrentAnimationStack);

	FbxTakeInfo* lCurrentTakeInfo = m_Scene->GetTakeInfo(*(m_AnimStackNameArray[index]));
	if (lCurrentTakeInfo)
	{
		m_Start = lCurrentTakeInfo->mLocalTimeSpan.GetStart();
		m_Stop = lCurrentTakeInfo->mLocalTimeSpan.GetStop();
	}
	else
	{
		// Take the time line value
		FbxTimeSpan lTimeLineTimeSpan;
		m_Scene->GetGlobalSettings().GetTimelineDefaultTimeSpan(lTimeLineTimeSpan);

		m_Start = lTimeLineTimeSpan.GetStart();
		m_Stop = lTimeLineTimeSpan.GetStop();
	}

	// check for smallest start with cache start
	if (m_CacheStart < m_Start)
		m_Start = m_CacheStart;

	// check for biggest stop with cache stop
	if (m_CacheStop  > m_Stop)
		m_Stop = m_CacheStop;

	// move to beginning
	m_CurrentTime = m_Start;

	return true;
}
const string& CFbxModel::GetName() const
{
	return m_Name;
}

void CFbxModel::OnTimerClick() const
{
	// Loop in the animation stack if not paused.
	if (m_Stop > m_Start)
	{
		m_CurrentTime += m_FrameTime;

		if (m_CurrentTime > m_Stop)
		{
			m_CurrentTime = m_Start;
		}
	}
}

void CFbxModel::CleanUp()
{
	FbxArrayDelete(m_AnimStackNameArray);

	// Unload the cache and free the memory
	if (m_Scene)
	{
		UnloadCacheRecursive(m_Scene);
	}

	// Delete the FBX SDK manager. All the objects that have been allocated 
	// using the FBX SDK manager and that haven't been explicitly destroyed 
	// are automatically destroyed at the same time.
	DestroySdkObjects(m_SdkManager, true);
}
void CFbxModel::UnloadCacheRecursive(FbxNode * pNode)
{
	// Unload the material cache
	const int lMaterialCount = pNode->GetMaterialCount();
	for (int lMaterialIndex = 0; lMaterialIndex < lMaterialCount; ++lMaterialIndex)
	{
		FbxSurfaceMaterial * lMaterial = pNode->GetMaterial(lMaterialIndex);
		if (lMaterial && lMaterial->GetUserDataPtr())
		{
			SMaterial* lMaterialCache = static_cast<SMaterial *>(lMaterial->GetUserDataPtr());
			lMaterial->SetUserDataPtr(NULL);
			delete lMaterialCache;
		}
	}

	FbxNodeAttribute* lNodeAttribute = pNode->GetNodeAttribute();
	if (lNodeAttribute)
	{
		// Unload the mesh cache
		if (lNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			FbxMesh * lMesh = pNode->GetMesh();
			if (lMesh && lMesh->GetUserDataPtr())
			{
				CMesh* lMeshCache = static_cast<CMesh*>(lMesh->GetUserDataPtr());
				lMesh->SetUserDataPtr(NULL);
				lMeshCache->CleanUp();
				delete lMeshCache;
			}
		}
	}

	const int lChildCount = pNode->GetChildCount();
	for (int lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		UnloadCacheRecursive(pNode->GetChild(lChildIndex));
	}
}
void CFbxModel::UnloadCacheRecursive(FbxScene * pScene)
{
	const int lTextureCount = pScene->GetTextureCount();
	for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
	{
		FbxTexture * lTexture = pScene->GetTexture(lTextureIndex);
		FbxFileTexture * lFileTexture = FbxCast<FbxFileTexture>(lTexture);
		if (lFileTexture && lFileTexture->GetUserDataPtr())
		{
			GLuint * lTextureName = static_cast<GLuint *>(lFileTexture->GetUserDataPtr());
			lFileTexture->SetUserDataPtr(NULL);
			glDeleteTextures(1, lTextureName);
			delete lTextureName;
		}
	}

	UnloadCacheRecursive(pScene->GetRootNode());
}
void CFbxModel::DestroySdkObjects(FbxManager* pManager, bool pExitStatus)
{
	//Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
	if (pManager) pManager->Destroy();
	if (pExitStatus) FBXSDK_printf("FBXLoader Program Success end!\n");
}

FbxDouble3 CFbxModel::GetMaterialProperty(	const FbxSurfaceMaterial*	material,
											const char*					property_name,
											const char*					factor_property_name,
											GLuint&						texture_id)
{
	FbxDouble3 lResult(0, 0, 0);
	const FbxProperty lProperty = material->FindProperty(property_name);
	const FbxProperty lFactorProperty = material->FindProperty(factor_property_name);
	if (lProperty.IsValid() && lFactorProperty.IsValid())
	{
		lResult = lProperty.Get<FbxDouble3>();
		double lFactor = lFactorProperty.Get<FbxDouble>();
		if (lFactor != 1)
		{
			lResult[0] *= lFactor;
			lResult[1] *= lFactor;
			lResult[2] *= lFactor;
		}
	}

	if (lProperty.IsValid())
	{
		const int lTextureCount = lProperty.GetSrcObjectCount<FbxFileTexture>();
		if (lTextureCount)
		{
			const FbxFileTexture* lTexture = lProperty.GetSrcObject<FbxFileTexture>();
			if (lTexture && lTexture->GetUserDataPtr())
			{
				texture_id = *(static_cast<GLuint *>(lTexture->GetUserDataPtr()));
			}
		}
	}

	return lResult;
}