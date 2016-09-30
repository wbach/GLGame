#include "Loader.h"
CLoader::CLoader()
: m_TextureLoader(m_Textures)
{
}


void CLoader::CleanUp()
{
	for (shared_ptr<CModel>& model : m_Models)
		model->CleanUp();
	
	for (STextInfo& text : m_Textures)
		text.CleanUp();

	m_Textures.clear();
	m_Models.clear();
}
int CLoader::LoadMesh(string file_name)
{
	std::ifstream try_file(file_name);
	if (!try_file.is_open()) {
		std::cout << "[Error] The file " << file_name << " wasnt successfuly opened " << std::endl;
		return -1;
	}
	try_file.close();

	std::string extenstion = file_name.substr(file_name.find_last_of(".") + 1);
	
	string path = file_name.substr(0, file_name.find_last_of('/'));

	for (unsigned int x = 0; x < m_Models.size(); x++)
		if (file_name.compare(m_Models[x]->GetName()) == 0)
			return x;

	if (!extenstion.compare("fbx") || !extenstion.compare("FBX")|| !extenstion.compare("Fbx"))
	{
		shared_ptr<CModel> model = make_shared<CFbxModel>(m_TextureLoader);
		model->InitModel(file_name);
		m_Models.push_back(model);
		return m_Models.size() - 1;
	}
	else
	{
		shared_ptr<CModel> model = make_shared<CAssimModel>(m_TextureLoader);
		model->InitModel(file_name);
		m_Models.push_back(model);
		return m_Models.size() - 1;
	}

	return -1;
}

GLuint CLoader::LoadTexture(string file_name, bool vertical_flip)
{
	return m_TextureLoader.LoadTexture(file_name, vertical_flip);
}
