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
	m_IndexesUpdatingModels.clear();
}
void CLoader::SetMaxTextureResolution(const glm::vec2& resolution)
{
	m_TextureLoader.SetMaxTextureResolution(resolution);
}
void CLoader::UpdateModels(float delta_time)
{
	for (int index : m_IndexesUpdatingModels)
	{
		m_Models[index]->Update(delta_time);
	}
}
int CLoader::LoadMesh(string file_name,bool time_update)
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
	shared_ptr<CModel> model;
	if (!extenstion.compare("fbx") || !extenstion.compare("FBX")|| !extenstion.compare("Fbx"))
		model = make_shared<CFbxModel>(m_TextureLoader);
	else
		model = make_shared<CAssimModel>(m_TextureLoader);

	model->InitModel(file_name);
	m_Models.push_back(model);
	if(time_update)
		m_IndexesUpdatingModels.push_back(m_Models.size() - 1);

	return m_Models.size() - 1;
}

GLuint CLoader::LoadTexture(string file_name, bool vertical_flip)
{
	return m_TextureLoader.LoadTexture(file_name, vertical_flip);
}

GLuint CLoader::LoadFullTexture(string file_name, GLubyte *& data, int & width, int & height)
{
	return m_TextureLoader.LoadFullTexture(file_name, true, data, width, height);
}

GLuint CLoader::LoadCubeMap(const vector<string>& filenames)
{
	return m_TextureLoader.LoadCubeMap(filenames);
}

void CLoader::ReloadTexture(string file_name, GLuint& texture_id)
{
	m_TextureLoader.ReloadTexture(file_name, &texture_id);
}

void CLoader::ReloadTexture(GLubyte* data, GLuint& texture_id, int width, int height)
{
	m_TextureLoader.ReloadTexture(data, texture_id, width, height);
}

void CLoader::UpdateSubTexture(GLuint & texture_id, GLubyte * subdata, int start_x, int start_y, int width, int height)
{
	m_TextureLoader.UpdateSubTexture(texture_id, subdata, start_x, start_y, width, height);
}

void CLoader::SaveTextureToFile(string file_name, GLubyte * data, int width, int height)
{
	m_TextureLoader.SaveTextureToFile(file_name, data, width, height);
}
