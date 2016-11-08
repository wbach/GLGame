#include "Loader.h"
CLoader::CLoader()
: m_TextureLoader(m_Textures)
, m_UpdateThreadRun(false)
{
}

void CLoader::CleanUp()
{
	m_UpdateThreadRun = false;
	//animation_thread.join();

	for (auto& model : m_Models)
	{
		model->CleanUp();
	}
	
	for (auto& text : m_Textures)
		text.CleanUp();

	m_Textures.clear();
	m_Models.clear();
	m_IndexesUpdatingModels.clear();
}
void CLoader::SetMaxTextureResolution(const glm::vec2& resolution)
{
	m_TextureLoader.SetMaxTextureResolution(resolution);
}
void CLoader::CreateUpdateThread()
{
	animation_thread =  std::thread(&CLoader::UpdateModelsThread, this);
	//animation_thread.detach();
}
void CLoader::AddModelToUpdate(const unsigned int& eid, const unsigned int& mid)
{
	m_IndexesInFrame.insert({ eid, mid });
}
void CLoader::UpdateModels(float delta_time)
{
	//cout << m_IndexesInFrame.size() << endl;
	std::unordered_map<int, int> entities_map_update;
	for (const auto& n : m_IndexesInFrame)
	{
		if (!entities_map_update.insert({ n.first, 0 }).second)
			continue;
		if (!entities_map_update.insert({ n.second, 0 }).second)
			continue;
		m_Models[n.second]->Update(delta_time);
	}
	m_IndexesInFrame.clear();
}
void CLoader::UpdateModelsThread()
{
	int m_FrameCount = 0;
	float m_Fps = 0;
	float m_CurrentTime = 0, m_PreviousTime = 0;
	float time = 0;
	m_UpdateThreadRun = true;
	while (m_UpdateThreadRun)
	{
		auto start = std::chrono::high_resolution_clock::now();
		 
		cout << "Animation to fps: " << m_Fps << endl;
		for (const auto& n : m_IndexesInFrame)
		{
			m_Models[n.first]->Update(time);
		}
		m_IndexesInFrame.clear();
		
		time += 0.1;

		if (time > 1000)
			time = 0;


			

			m_FrameCount++;

		//	m_CurrentTime = m_DisplayManager.GetCurrentTime();

			int time_interval = static_cast<int>(m_CurrentTime) - static_cast<int>(m_PreviousTime);

			if (time_interval > 1)
			{
				m_Fps = static_cast<float>(m_FrameCount) / static_cast<float>(time_interval);
				m_PreviousTime = m_CurrentTime;
				m_FrameCount = 0;
			}
			auto end = std::chrono::high_resolution_clock::now();

			if (std::chrono::milliseconds(16) >  std::chrono::duration_cast<std::chrono::milliseconds>(end - start))
				std::this_thread::sleep_for(std::chrono::milliseconds(16) - std::chrono::duration_cast<std::chrono::milliseconds>(end - start));
	}
	
}
int CLoader::LoadMesh(string file_name, bool time_update)
{
	if (!Utils::CheckFile(file_name)) {
		std::cout << "[Error] The file " << file_name << " wasnt successfuly opened " << std::endl;
		return -1;
	}

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
	model->m_TimeUpdate = time_update;
	model->SetName(file_name);
	m_Models.push_back(model);
	model->SetId(m_Models.size() - 1);
	//if(time_update)
	//	m_IndexesUpdatingModels.push_back(m_Models.size() - 1);

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

void CLoader::CreateEmptyImage(const std::string filename, int width, int height)
{
	m_TextureLoader.CreateEmptyImage(filename, width, height);
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

std::vector<SFace> CLoader::LoadFaces(const std::string & filename) const
{
	std::vector<SFace> faces;

	vector<float> positions;
	vector<float> normals;
	vector<unsigned int> indices;

	CAssimModel::ReadCollisions(filename, positions, normals, indices);

	if (indices.size() < 3)
		return faces;

	vector<glm::vec3> norm, vertexes;
	for (unsigned int x = 0; x < positions.size(); x += 3)
	{
		float& xx = positions[x];
		float& yy = positions[x + 1];
		float& zz = positions[x + 2];
		vertexes.push_back(glm::vec3(xx, yy, zz));
	}
	for (unsigned int x = 0; x < normals.size(); x += 3)
	{
		norm.push_back(glm::vec3(normals[x], normals[x + 1], normals[x + 2]));
	}
	for (unsigned int x = 0; x < indices.size(); x += 3)
	{
		SFace face;
		face.vertexes.v1 = vertexes[indices[x]];
		face.vertexes.v2 = vertexes[indices[x + 1]];
		face.vertexes.v3 = vertexes[indices[x + 2]];
		face.CalculateNormal();
		faces.push_back(face);
	}
	return faces;
}

CTextureLoader& CLoader::GetTextureLoader()
{
	return m_TextureLoader;
}

void CLoader::SaveTextureToFile(string file_name, GLubyte * data, int width, int height)
{
	m_TextureLoader.SaveTextureToFile(file_name, data, width, height);
}
