#include "AssimpModel.h"

CAssimModel::CAssimModel(CTextureLoader & texture_lodaer)
: m_TextureLodaer(texture_lodaer)
{
}

void CAssimModel::InitModel(string file_name)
{
	string path = file_name.substr(0, file_name.find_last_of('/'));

	m_LoaderType == LoaderType::ASSIMP; 

	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
	//if(normals == "flat" ) flags |= aiProcess_GenNormals ;
	//if(normals == "smooth" ) flags |= aiProcess_GenSmoothNormals ;

	const aiScene *scene = importer.ReadFile(file_name.c_str(), flags);
	if (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "[Error] The file " << file_name << " wasnt successfuly opened " << std::endl;
		return;
	}
	RecursiveProcess(path + "/", scene->mRootNode, scene);
}

const string& CAssimModel::GetName() const
{
	return m_Name;
}

void CAssimModel::CleanUp()
{
	for (CMesh& mesh : m_Meshes)
	{
		mesh.CleanUp();
	}
}
void CAssimModel::ProcessMesh(string file_path, aiMesh* mesh, const aiScene* scene)
{
	vector<float> postions;
	vector<float> text_coords;
	vector<float> normals;
	vector<float> tangents;
	vector<float> diffuse;
	vector<float> specular;
	vector<float> ambient;
	vector<unsigned int> indices;

	aiColor4D diff;
	aiColor4D amb;
	aiColor4D spec;
	float shine_damper;
	float transparent;
	float reflectivity;
	aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
	aiString name;
	aiGetMaterialString(mat, AI_MATKEY_NAME, &name);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diff);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &amb);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &spec);
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shine_damper);
	aiGetMaterialFloat(mat, AI_MATKEY_REFLECTIVITY, &reflectivity);
	aiGetMaterialFloat(mat, AI_MATKEY_OPACITY, &transparent);

	glm::vec3 defaultDiff(diff.r, diff.g, diff.b);
	glm::vec3 defaultSpec(spec.r, spec.g, spec.b);
	glm::vec3 defaultAmb(amb.r, amb.g, amb.b);

	for (unsigned int i = 0; i< mesh->mNumVertices; i++)
	{

		postions.push_back(mesh->mVertices[i].x);
		postions.push_back(mesh->mVertices[i].y);
		postions.push_back(mesh->mVertices[i].z);


		normals.push_back(mesh->mNormals[i].x);
		normals.push_back(mesh->mNormals[i].y);
		normals.push_back(mesh->mNormals[i].z);


		if (mesh->mTangents)
		{
			tangents.push_back(mesh->mTangents[i].x);
			tangents.push_back(mesh->mTangents[i].y);
			tangents.push_back(mesh->mTangents[i].z);

		}
		else
		{
			tangents.push_back(1.0f);
			tangents.push_back(0.0f);
			tangents.push_back(0.0f);
		}

		if (mesh->mColors[0])
		{
			defaultDiff.x = mesh->mColors[0][i].r;
			defaultDiff.y = mesh->mColors[0][i].g;
			defaultDiff.z = mesh->mColors[0][i].b;
		}

		if (mesh->mTextureCoords[0])
		{
			text_coords.push_back(mesh->mTextureCoords[0][i].x);
			text_coords.push_back(mesh->mTextureCoords[0][i].y);
		}
		else
		{
			text_coords.push_back(0);
			text_coords.push_back(0);
		}
	}

	for (unsigned int i = 0; i<mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j<face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	SMaterial material;
	material.diffuse = defaultDiff;
	material.specular = defaultSpec;
	material.shineDamper = shine_damper;
	material.reflectivity = reflectivity;

	for (unsigned int i = 0; i <mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
		STextInfo texture;
		texture.id = m_TextureLodaer.LoadTexture(file_path + str.C_Str());
		texture.type = MaterialTexture::DIFFUSE;
		material.textures.push_back(texture);
	}
	for (unsigned int i = 0; i <mat->GetTextureCount(aiTextureType_HEIGHT); i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_HEIGHT, i, &str);
		STextInfo texture;
		texture.id = m_TextureLodaer.LoadTexture(file_path + str.C_Str());
		texture.type = MaterialTexture::NORMAL;
		material.textures.push_back(texture);
	}
	AddMesh(file_path, postions, text_coords, normals, tangents, indices, material);
}

void CAssimModel::RecursiveProcess(string file_path, aiNode *node, const aiScene *scene)
{
	//proces
	for (unsigned int i = 0; i<node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(file_path, mesh, scene);
	}
	//r
	for (unsigned int i = 0; i <node->mNumChildren; i++)
	{
		RecursiveProcess(file_path, node->mChildren[i], scene);
	}
}