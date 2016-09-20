#include "Loader.h"
GLuint CLoader::LoadFullTexture(string file_name, bool keepData, GLubyte *&texture, int &width, int &height, bool vertical_flip, float quality)
{
	cout << "Loading texture : " << file_name << endl;

	string file = file_name.substr(file_name.find_last_of('/') + 1);

	for (const STextInfo& t : m_Textures)
	{
		if (file.compare(t.filename) == 0) 
			return t.id;		
	}
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(file_name.c_str(), 0);

	if (formato == FIF_UNKNOWN) { printf("%s : wrong image format or file does not exist.\n", file_name.c_str()); return 0; }
	FIBITMAP* imagen2 = FreeImage_Load(formato, file_name.c_str());
	if (!imagen2) { printf("%s : wrong image format or file does not exist.\n", file_name.c_str()); return 0; }
	FIBITMAP* imagen = FreeImage_ConvertTo32Bits(imagen2);	
	FreeImage_Unload(imagen2);

	if(vertical_flip)
		FreeImage_FlipVertical(imagen);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	if (quality < 1)
	{
		w = static_cast<int>(w / quality);
		h = static_cast<int>(h / quality);
		imagen = FreeImage_Rescale(imagen, w, h, FILTER_BILINEAR);
	}
	
	if (keepData) {
		FreeImage_FlipVertical(imagen);
	}
	char* pixeles = (char*)FreeImage_GetBits(imagen);

	texture = new GLubyte[4 * w*h];

	//bgr2rgb
	for (int j = 0; j<w*h; j++)
	{
		texture[j * 4 + 0] = pixeles[j * 4 + 2];
		texture[j * 4 + 1] = pixeles[j * 4 + 1];
		texture[j * 4 + 2] = pixeles[j * 4 + 0];
		texture[j * 4 + 3] = pixeles[j * 4 + 3];
	}

	if (keepData) {
		width = w;
		height = h;
		FreeImage_Unload(imagen);
		return -1;
	}

	// Create one OpenGL texture
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	GLenum hubo_error = glGetError();
	if (hubo_error)
	{
		printf("%s : There was an error loading the texture : %s\n", file_name.c_str(), glewGetErrorString(hubo_error));
		delete[] texture;
		return 0;
	}
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, texture_id);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	delete[] texture;
	FreeImage_Unload(imagen);

	STextInfo text;
	text.id = texture_id;
	text.filename = file;
	m_Textures.push_back(text);
	return texture_id;
}

GLuint CLoader::LoadTexture(string filename, bool verticalFlip)
{
	int w, h; GLubyte* data;
	return LoadFullTexture(filename, false, data, w, h, verticalFlip);
}


void CLoader::ProcessMesh(CModel &model,string file_path, aiMesh *mesh, const aiScene *scene)
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
	aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
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

	//if(spec.r > 0.5)		cout << spec.r << " " << spec.g << " " << spec.b <<endl;
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
		texture.id = LoadTexture(file_path + str.C_Str());
		texture.type = MaterialTexture::DIFFUSE;
		material.textures.push_back(texture);
	}
	for (unsigned int i = 0; i <mat->GetTextureCount(aiTextureType_HEIGHT); i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_HEIGHT, i, &str);
		STextInfo texture;
		texture.id = LoadTexture(file_path + str.C_Str());
		texture.type = MaterialTexture::NORMAL;
		material.textures.push_back(texture);
	}
	model.AddMesh(file_path, postions, text_coords, normals, tangents, indices, material);
}

void CLoader::RecursiveProcess(CModel &model,string file_path, aiNode *node, const aiScene *scene)
{
	//proces
	for (unsigned int i = 0; i<node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(model, file_path, mesh, scene);
	}

	//r
	for (unsigned int i = 0; i <node->mNumChildren; i++)
	{
		RecursiveProcess(model, file_path, node->mChildren[i], scene);
	}
}

void CLoader::CleanUp()
{
	for (CModel& model : m_Models) 
		model.CleanUp();
	
	for (STextInfo& text : m_Textures)
		text.CleanUp();

	m_Textures.clear();
	m_Models.clear();
}

int CLoader::AssimpLoad(string file_name)
{
	std::ifstream try_file(file_name);
	if(!try_file.is_open()){
        std::cout << "The file " << file_name <<" wasnt successfuly opened ";
		return -1;
	}
	try_file.close();

	string path = file_name.substr(0, file_name.find_last_of('/'));

	for (unsigned int x = 0; x < m_Models.size(); x++)
		if (file_name.compare(m_Models[x].GetName()) == 0)
			return x;

	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
	//if(normals == "flat" ) flags |= aiProcess_GenNormals ;
	//if(normals == "smooth" ) flags |= aiProcess_GenSmoothNormals ;

	const aiScene *scene = importer.ReadFile(file_name.c_str(), flags);
	if (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "The file %s wasnt successfuly opened " << file_name;
		return -1;
	}

	CModel model;
	RecursiveProcess(model,path + "/", scene->mRootNode, scene);
	m_Models.push_back(model);
	return m_Models.size()-1;
}
