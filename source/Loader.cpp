#include "Loader.h"
GLuint CLoader::loadFullTexture(string filename, bool keepData, GLubyte *&texture, int &width, int &height, bool verticalFlip) {

	cout << "Loading texture : " << filename << endl;

	string file = filename.substr(filename.find_last_of('/') + 1);
	//system("cls");
	//printf("%s\n",file.c_str());


	//	printf("######################################\n");
	for (TextInfo t : textures) {
		//printf("%s\n",t.filename.c_str());
		if (file.compare(t.filename) == 0) {
			return t.id;
		}
	}
	//printf("######################################\n");
	//system("pause");
	FREE_IMAGE_FORMAT formato = FreeImage_GetFileType(filename.c_str(), 0);

	if (formato == FIF_UNKNOWN) { printf("%s : wrong image format or file does not exist.\n", filename.c_str()); return 0; }
	FIBITMAP* imagen2 = FreeImage_Load(formato, filename.c_str());
	if (!imagen2) { printf("%s : wrong image format or file does not exist.\n", filename.c_str()); return 0; }
	FIBITMAP* imagen = FreeImage_ConvertTo32Bits(imagen2);	
	FreeImage_Unload(imagen2);

	//FIBITMAP temp = imagen;

	//FreeImage_Unload(temp);
	if(verticalFlip)	FreeImage_FlipVertical(imagen);

	int w = FreeImage_GetWidth(imagen);
	int h = FreeImage_GetHeight(imagen);

	//imagen = FreeImage_Rescale(imagen, w / 4, h / 4, FILTER_BILINEAR);

	//w /= 4;
	//h /= 4;
	//printf( "%s : wrong image format or file does not exist.",filename);
	//cout << "The size of the image in " << filePath << " is: " << w << "x" << h <<endl; //Some debugging code
	if (keepData) {
		FreeImage_FlipVertical(imagen);
		//FreeImage_FlipHorizontal(imagen);

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
	GLuint textureID;
	glGenTextures(1, &textureID);
	GLenum huboError = glGetError();
	if (huboError) {
		printf("%s : There was an error loading the texture : %s\n", filename.c_str(), glewGetErrorString(huboError));
		delete[] texture;
		return 0;
	}
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
	delete[] texture;
	FreeImage_Unload(imagen);

	TextInfo text;
	text.id = textureID;
	text.filename = file;
	textures.push_back(text);
	return textureID;
}

GLuint CLoader::loadTexture(string filename, bool verticalFlip)
{
	int w, h; GLubyte* data;
	return loadFullTexture(filename, false, data, w, h, verticalFlip);
}


void CLoader::processMesh(CModel &model,string filepath, aiMesh *mesh, const aiScene *scene)
{
	vector<float>postions;
	vector<float>textCoords;
	vector<float>normals;
	vector<float>tangents;
	vector<float>diffuse;
	vector<float>specular;
	vector<float>ambient;
	vector<unsigned int> indices;

	aiColor4D diff;
	aiColor4D amb;
	aiColor4D spec;
	float shineDamper;
	float transparent;
	float reflectivity;
	aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
	aiString name;
	aiGetMaterialString(mat, AI_MATKEY_NAME, &name);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diff);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &amb);
	aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &spec);
	aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shineDamper);
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
			textCoords.push_back(mesh->mTextureCoords[0][i].x);
			textCoords.push_back(mesh->mTextureCoords[0][i].y);
			//tmpVec.z = mesh->mTextureCoords[0][i].z ;
		}
		else
		{
			textCoords.push_back(0);
			textCoords.push_back(0);
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


	Material material;
	material.diffuse = defaultDiff;
	material.specular = defaultSpec;
	material.shineDamper = shineDamper;
	material.reflectivity = reflectivity;



	for (unsigned int i = 0; i <mat->GetTextureCount(aiTextureType_DIFFUSE); i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE, i, &str);
		TextInfo texture;
		texture.id = loadTexture(filepath + str.C_Str());
		texture.type = DIFFUSE_TEXTURE;
		material.textures.push_back(texture);
	}
	for (unsigned int i = 0; i <mat->GetTextureCount(aiTextureType_HEIGHT); i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_HEIGHT, i, &str);
		TextInfo texture;
		texture.id = loadTexture(filepath + str.C_Str());
		texture.type = NORMAL_TEXTURE;
		material.textures.push_back(texture);
	}


	model.addMesh(filepath, postions, textCoords, normals, tangents,indices, material);

}

void CLoader::recursiveProcess(CModel &model,string filepath, aiNode *node, const aiScene *scene)
{
	//proces
	for (unsigned int i = 0; i<node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(model, filepath, mesh, scene);
	}

	//r
	for (unsigned int i = 0; i <node->mNumChildren; i++)
	{
		recursiveProcess(model, filepath, node->mChildren[i], scene);
	}
}

void CLoader::cleanUp()
{
	for (CModel model : models) {
		model.cleanUp();
	}
	for (TextInfo text : textures) {
		text.cleanUp();
	}
	textures.clear();
	models.clear();
}

int CLoader::assimpLoad(string filename)
{
	std::ifstream tryfile(filename);
	if(!tryfile.is_open()){
        std::cout << "The file " << filename <<" wasnt successfuly opened ";
		return -1;
	}
    tryfile.close();

	string path = filename.substr(0, filename.find_last_of('/'));

	for (int x = 0; x < models.size(); x++) {
		if (filename.compare(models[x].getName()) == 0) {
			return x;
		}
	}

	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals;
	//if(normals == "flat" ) flags |= aiProcess_GenNormals ;
	//if(normals == "smooth" ) flags |= aiProcess_GenSmoothNormals ;

	const aiScene *scene = importer.ReadFile(filename.c_str(), flags);
	if (scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "The file %s wasnt successfuly opened " << filename;
		return -1;
	}


	CModel model;
	recursiveProcess(model,path + "/", scene->mRootNode, scene);
	models.push_back(model);
	return models.size()-1;
}
