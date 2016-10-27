#include "GLgame.h"
#include "../Input/InputManager.h"
CGame::CGame()
: m_BackgroundColour(0.6, 0.6, 0.8)
, m_WindowSize(1000, 600)
, m_WindowName("GL game")
, m_IsSound(true)
, m_IsShadows(true)
, m_GrassViewDistance(500)
, m_IsFullScreen(false)
, m_RefreshRate(60)
, m_ShadowMapSize(1024)
, m_SoundVolume(1.0)
, m_WaterQuality(1)
, m_ViewDistance(1000)
, m_MaxTextureResolution(4096)
{
	ReadConfiguration("./Data/Conf.ini");
}

void CGame::Initialize(std::shared_ptr<CApi>& api)
{	
	m_DisplayManager.SetApi(api);
	m_DisplayManager.Initialize(m_WindowName, Renderer::OPENGL, static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));

	CreateProjectionMatrix();

	m_GuiRenderer.Init(static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));

	m_DisplayManager.SetInput(m_InputManager.m_Input);
	m_DisplayManager.SetFullScreen(m_IsFullScreen);
	m_DisplayManager.SetRefreshRate(m_RefreshRate);

	//renderStartSeries();
	LoadScene();
	m_MasterRenderer.Init(m_CurrScene->GetCamera().get(), m_WindowSize, m_ProjectionMatrix, 
						 m_Fov, m_NearPlane ,m_FarPlane, m_RenderingResolutionModifier, m_ShadowMapSize, m_ShadowsDistance,
						m_WaterQuality, m_ReflectionSize, m_RefractionSize, m_ViewDistance
						);
}
void CGame::Uninitialize()
{
	if (m_CurrScene != nullptr)
		m_CurrScene->CleanUp();

	OnGameLoopRun = nullptr;

	m_GuiRenderer.CleanUP();
	m_MasterRenderer.CleanUp();

	m_DisplayManager.Uninitialize();	
}
void CGame::SetWindowSize(glm::vec2 size)
{
	m_WindowSize = size;
}
void CGame::GameLoop()
{	
	m_ApiMessage = ApiMessages::NONE;

    string fps_text = "FPS : ";
	string object_count_text = "Objects : ";
	string vertex_count_text = "Vertex : ";
	vector<CGUIText> texts;

	glm::vec2 shadow_offset(-0.002);
	glm::vec2 fps_pos(-0.9475, 0.8575);
	texts.push_back(CGUIText(fps_text, fps_pos + shadow_offset, 1, glm::vec3(0, 0, 0)));
	texts.push_back(CGUIText(fps_text, fps_pos, 1, glm::vec3(0.9)));
	
	glm::vec2 oc(-0.9475, 0.7575);
	texts.push_back(CGUIText(object_count_text, oc + shadow_offset, 1, glm::vec3(0, 0, 0)));
	texts.push_back(CGUIText(object_count_text, oc, 1, glm::vec3(0.9)));

	glm::vec2 vc(-0.9475, 0.6575);
	texts.push_back(CGUIText(vertex_count_text, vc + shadow_offset, 1, glm::vec3(0, 0, 0)));
	texts.push_back(CGUIText(vertex_count_text, vc, 1, glm::vec3(0.9)));
	//texts.push_back(CGUIText("Lorem Ipsum is simply dummy text of the printing and typesetting industry.\n Lorem Ipsum has been the industry's standard dummy text ever since the 1500s.", glm::vec2(-0.90, 0.7), 1, glm::vec3(1, 1, 1)));
	
	
	vector<CGUITexture> debug_textures;
	//debug_textures.push_back(CGUITexture(m_MasterRenderer.GetShadowMap(), glm::vec2(0.5), glm::vec2(0.25,0.25) ));
	
	std::thread physics_thread(&CGame::PhysicsLoop, this);

	while (m_ApiMessage != ApiMessages::QUIT && !m_FroceQuit)
	{	
		m_ApiMessage = m_DisplayManager.PeekMessage();

		if (m_InputManager.GetKey(KeyCodes::ESCAPE))
			m_ApiMessage = ApiMessages::QUIT;

		if (OnGameLoopRun != nullptr)
			OnGameLoopRun();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(m_BackgroundColour.x, m_BackgroundColour.y, m_BackgroundColour.z, 1);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);	
	
		if (m_CurrScene != nullptr)
		{
			m_CurrScene->ApplyPhysicsToObjects(static_cast<float>(m_DisplayManager.GetDeltaTime()));
			switch (m_CurrScene->Update())
			{
			case 1: m_ApiMessage = ApiMessages::QUIT; break;
			case 2: m_CurrScene->CleanUp();  SetCurrentScene(1); LoadScene();  break;				
			}
			GetCurrentScene()->m_PhysicsScene.Update(static_cast<float>(m_DisplayManager.GetDeltaTime()));


			m_MasterRenderer.Render(m_CurrScene, m_IsShadows);
			//m_MasterRenderer.DebugRenderTextures();

			m_GuiRenderer.Render(m_CurrScene->GetGui());
			m_GuiRenderer.RenderTextures(debug_textures);
		}
		int objects_per_frame = m_MasterRenderer.GetObjectsPerFrame();
		int vertex_per_frame = m_MasterRenderer.GetVertexPerFrame();

		texts[0].updateText(fps_text + std::to_string(m_DisplayManager.GetFps()));
		texts[1].updateText(fps_text + std::to_string(m_DisplayManager.GetFps()));

		texts[2].updateText(object_count_text + std::to_string(objects_per_frame));
		texts[3].updateText(object_count_text + std::to_string(objects_per_frame));

		texts[4].updateText(vertex_count_text + std::to_string(vertex_per_frame));
		texts[5].updateText(vertex_count_text + std::to_string(vertex_per_frame));

		m_GuiRenderer.RenderText(texts);
		m_DisplayManager.Update();
	}
	physics_thread.detach();
}
void CGame::PhysicsLoop()
{
	int m_FrameCount = 0;
	float m_Fps = 0;
	float m_CurrentTime = 0, m_PreviousTime = 0;
	while (m_ApiMessage != ApiMessages::QUIT && !m_FroceQuit)
	{
		if (m_CurrScene == nullptr) continue;

		auto start = std::chrono::high_resolution_clock::now();

		m_FrameCount++;

		m_CurrentTime = m_DisplayManager.GetCurrentTime();

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
void CGame::LoadScene()
{
	if (m_CurrScene == nullptr) return;

	m_IsLoading = true;
//	thread loading_thread(&CGame::InitializeScene,this) ;

	m_CurrScene->Initialize();

	m_IsLoading = false;

	//loading_thread.join();
}
float CGame::Fade(float delta_time)
{
	bool change = false;
	float alpha = 0.0;
	if (delta_time < 500) change = true;
	if (delta_time < 2000 && delta_time > 500) 
	{

		alpha = (static_cast<float>(delta_time) - 500.0f) / 2000.0f;
		if (alpha > 1)
			alpha = 1.0;
		change = true;
	}
	if (delta_time > 4000 )
	{
		alpha = ((6000.0f - static_cast<float>(delta_time)) / 2000.0f);
		if (alpha < 0)
			alpha = 0;
		change = true;
	}
	if (!change)
		return 1.0;

	return alpha;
}
float CGame::FadeIn(float delta_time, float start_time, float durration)
{
	float alpha = 0.0;
	alpha = (static_cast<float>(delta_time) - static_cast<float>(start_time)) / static_cast<float>(durration);
	if (alpha > 1)
		alpha = 1.0;
	return alpha;
}
float CGame::FadeOut(float delta_time, float start_time, float durration)
{
	float alpha = 0.0;
	alpha = ((static_cast<float>(start_time + durration) - static_cast<float>(delta_time)) / static_cast<float>(durration));
	if (alpha < 0)
		alpha = 0;

	return alpha;
}
int CGame::ReadConfiguration(string file_name)
{
	ifstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		std::cout << "[Error] Cant open configuration file." << std::endl;
		return -1;
	}
	string line;

	while (std::getline(file, line))
	{
		string var = line.substr(0, line.find_last_of("="));
		string value = line.substr(line.find_last_of("=") + 1);

		if (var.compare("Name") == 0)				m_WindowName	= value; 
		if (var.compare("Resolution") == 0)			m_WindowSize	= Get::Vector2d(value);
		if (var.compare("FullScreen") == 0)			m_IsFullScreen	= Get::Boolean(value);
		if (var.compare("RefreshRate") == 0)		m_RefreshRate	= Get::Int(value);
		if (var.compare("Sound") == 0)				m_IsSound		= Get::Boolean(value);
		if (var.compare("SoundVolume") == 0)		m_SoundVolume	= Get::Float(value);
		if (var.compare("RenderingResolution") == 0)	m_RenderingResolutionModifier = Get::Float(value);
		if (var.compare("WaterQuality") == 0)		m_WaterQuality	= Get::Float(value);
		if(var.compare("WaterReflectionResolution") == 0)	m_ReflectionSize = Get::Vector2d(value);
		if(var.compare("WaterRefractionResolution") == 0)	m_RefractionSize = Get::Vector2d(value);
		if (var.compare("TextureMaxResolution") == 0)	m_MaxTextureResolution = Get::Vector2d(value);
		if (var.compare("Shadows") == 0)			m_IsShadows		= Get::Boolean(value);
		if (var.compare("ShadowsDistance") == 0)	m_ShadowsDistance = Get::Float(value);
		if (var.compare("ShadowMapSize") == 0)		m_ShadowMapSize	= Get::Float(value);
		if (var.compare("ViewDistance") == 0)		m_ViewDistance  = Get::Float(value);
		if (var.compare("GrassViewDistance") == 0)	m_GrassViewDistance = Get::Float(value);
	}	
	file.close();

	return 0;
	//FILE *f = fopen("Data/Conf.ini", "r");
	//if (f == NULL)
	//{
	//	return -1;
	//}
	//int k;
	//fscanf(f, "Resolution=%ix%i\n", &m_WindowSize.x, &m_WindowSize.y);
	//fscanf(f, "FullScreen=%i\n", &k); if (k == 1) m_IsFullScreen = true; else m_IsFullScreen = false;
	//fscanf(f, "Sound=%i\n", &k); if (k == 1) m_IsSound = true; else m_IsSound = false;
	//fscanf(f, "SoundVolume=%f\n", &m_SoundVolume);
	//fscanf(f, "WaterQuality=%i\n", &m_WaterQuality); 
	//fscanf(f, "Shadows=%i\n", &k); if (k == 1) m_IsShadows = true; else m_IsShadows = false;
	//fscanf(f, "ShadowsResolution=%f\n", &m_ShadowMapSize);
	//fscanf(f, "ViewDistance=%f\n", &m_ViewDistance);
	//fscanf(f, "GrassViewDistance=%f\n", &m_GrassViewDistance);
	//fclose(f);
	return 0;
}
CScene* CGame::GetCurrentScene()
{
	return m_CurrScene;
}

const float & CGame::GetShadowMapSize()
{
	return m_ShadowMapSize;
}

const bool & CGame::IsShadows()
{
	return m_IsShadows;
}

const glm::vec2 & CGame::GetMaxTextureResolution()
{
	return m_MaxTextureResolution;
}


void CGame::RenderStartSeries()
{
	//vector<float> vertex = { -0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0 };
	//vector<float> text_coords = {
	//	0, 0,
	//	0, 1,
	//	1, 1,
	//	1, 0 };
	//vector<unsigned int>indices = { 0, 1, 3, 3, 1, 2 };

	//GLuint vao		   = Utils::CreateVao();
	//GLuint i_vbo	   = Utils::BindIndicesBuffer(indices);
	//GLuint vbo_id	   = Utils::StoreDataInAttributesList(0, 3, vertex);
	//GLuint vbo_text_id = Utils::StoreDataInAttributesList(1, 2, text_coords);
	//Utils::UnbindVao();
	//m_LoadingShader.Init();

	//GLuint texture = m_CurrScene->GetLoader().LoadTexture("Data/GUI/start1.png",true);
	//glm::mat4 transformation_matrix = Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(2.0));

	//Uint32 start,start2 = SDL_GetTicks();

	//Uint32 delta_time;
	//while (1) 
	//{
	//	delta_time = SDL_GetTicks() - start2;
	//	if (delta_time > 6000) break;
	//	start = SDL_GetTicks();

	//	glEnable(GL_DEPTH_TEST);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glClearColor(0, 0, 0, 1);

	//	m_LoadingShader.Start();

	//	if (delta_time > 500 && delta_time < 2500) {
	//		m_LoadingShader.LoadAlphaValue(FadeIn(delta_time, 500, 2000));
	//	}
	//	if (delta_time > 4000) {
	//		m_LoadingShader.LoadAlphaValue(FadeOut(delta_time, 4000, 2000));
	//	}

	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//	m_LoadingShader.LoadTransformMatrix(transformation_matrix);
	//	glBindVertexArray(vao);
	//	glEnableVertexAttribArray(0);
	//	glEnableVertexAttribArray(1);
	//	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//	glDisableVertexAttribArray(1);
	//	glDisableVertexAttribArray(0);
	//	glBindVertexArray(0);
	//	m_LoadingShader.Stop();
	//	m_DisplayManager.Update();

	//	if ( static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) > SDL_GetTicks() - start)  SDL_Delay(static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) - (SDL_GetTicks() - start));
	//}
	//m_LoadingShader.Stop();
	//m_LoadingShader.CleanUp();
	//glDeleteTextures(1, &texture);
	//glDeleteBuffers(1, &i_vbo);
	//glDeleteBuffers(1, &vbo_id);
	//glDeleteBuffers(1, &vbo_text_id);
	//glDeleteVertexArrays(1, &vao);
}
void CGame::InitializeScene()
{
//	SDL_GLContext gl_loading_context = SDL_GL_CreateContext(m_DisplayManager.GetWindow());
	//CGUIRenderer grenderer;
	//grenderer.Init(static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));

	//SDL_Event event;
	//float green = 1.0f;
	//float value = 0.01f;

	//vector<float> vertex = { -0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0};
	//vector<float> text_coords = {
	//	0, 0,
	//	0, 1,
	//	1, 1,
	//	1, 0};
	//vector<unsigned int> indices = {0, 1, 3, 3, 1, 2};

	//GLuint vao			= Utils::CreateVao();
	//GLuint i_vbo		= Utils::BindIndicesBuffer(indices);
	//GLuint vbo_id		= Utils::StoreDataInAttributesList(0, 3, vertex);
	//GLuint vbo_text_id  = Utils::StoreDataInAttributesList(1, 2, text_coords);
	//Utils::UnbindVao();
	//m_LoadingShader.Init();

	//GLuint texture = m_CurrScene->GetLoader().LoadTexture("Data/GUI/circle2.png");
	//m_CurrScene->GetLoader().m_Textures.clear();
	//glm::mat4 transformation_matrix = Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(0.25));

	//string loading_text = "Loading";
	//vector<CGUIText> texts;
	//texts.push_back(CGUIText(loading_text, glm::vec2(-0.15,-0.25), 2, glm::vec3(1, 1, 1)));

	//Uint32 xx = 0;

	//Uint32 start, start2 = SDL_GetTicks();

	//Uint32 delta_time;

	//int numPtr = 0;

	//bool ending_fade = false;
	//while (1)
	//{
	//	start = SDL_GetTicks();
	//	delta_time = SDL_GetTicks() - start2;

	//	glEnable(GL_DEPTH_TEST);
	//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	////	glClearColor(backgroundColour.x, green, backgroundColour.z, 1);
	//	glClearColor(0, 0, 0, 1);

	//	m_LoadingShader.Start();

	//	if (delta_time > 0 && delta_time < 2000)
	//	{
	//		m_LoadingShader.LoadAlphaValue(FadeIn(delta_time, 0, 1500));
	//	}

	//	if (!m_IsLoading)
	//	{
	//		m_IsLoading = true;
	//		ending_fade = true;
	//		start2 = SDL_GetTicks();
	//		delta_time = SDL_GetTicks() - start2;
	//	}
	//	if (ending_fade)
	//	{
	//		if (delta_time > 2000) break;
	//		m_LoadingShader.LoadAlphaValue(FadeOut(delta_time, 0, 2000));
	//	}

	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, texture);
	//	transformation_matrix *= glm::rotate(-1.0f, 0.0f, 0.0f, 1.0f);
	//	m_LoadingShader.LoadTransformMatrix(transformation_matrix);
	//	glBindVertexArray(vao);
	//	glEnableVertexAttribArray(0);
	//	glEnableVertexAttribArray(1);
	//	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	//	glDisableVertexAttribArray(1);
	//	glDisableVertexAttribArray(0);
	//	glBindVertexArray(0);

	//	while (SDL_PollEvent(&event))
	//	{
	//		switch (event.type)
	//		{
	//		case SDL_QUIT: m_IsLoading = false; break;
	//		case SDL_KEYDOWN:
	//			switch (event.key.keysym.sym)
	//			{
	//			case SDLK_F9: m_DisplayManager.SetFullScreen(true); break;
	//			case SDLK_ESCAPE: m_IsLoading = false; break;
	//			}
	//			break;
	//		}
	//	}
	//	/*green += value;
	//	if (green > 1.0 || green < 0) value = -value;*/
	//	//grenderer.render(currScene->getGUI().guiTextures);
	//	grenderer.RenderText(texts);
	//	m_DisplayManager.Update();

	//	if ( start - xx > 500) 
	//	{
	//		xx = start;
	//		numPtr++;
	//		loading_text += ".";
	//		if (numPtr > 3) {
	//			loading_text = "Loading";
	//			numPtr = 0;
	//		}
	//		texts[0].updateText(loading_text);
	//	}


	//	if (static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) > SDL_GetTicks() - start)  SDL_Delay(static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) - (SDL_GetTicks() - start));
	//}
	//m_LoadingShader.Stop();
	//m_LoadingShader.CleanUp();
	//glDeleteTextures(1, &texture);
	//glDeleteBuffers(1, &i_vbo);
	//glDeleteBuffers(1, &vbo_id);
	//glDeleteBuffers(1, &vbo_text_id);
	//glDeleteVertexArrays(1, &vao);

	//grenderer.CleanUP();
//	SDL_GL_DeleteContext(gl_loading_context);
}
void CGame::ClearScenes()
{
	m_Scenes.clear();
}
void CGame::CreateProjectionMatrix()
{
	float aspect_ratio = (float)m_WindowSize.x / (float)m_WindowSize.y;
	float y_scale = (float)((1.0f / tan(Utils::ToRadians(m_Fov / 2.0f))));
	float x_scale = y_scale / aspect_ratio;
	float frustum_length = m_FarPlane - m_NearPlane;

	m_ProjectionMatrix[0][0] = x_scale;
	m_ProjectionMatrix[1][1] = y_scale;
	m_ProjectionMatrix[2][2] = -((m_FarPlane + m_NearPlane) / frustum_length);
	m_ProjectionMatrix[2][3] = -1;
	m_ProjectionMatrix[3][2] = -((2 * m_NearPlane * m_FarPlane) / frustum_length);
	m_ProjectionMatrix[3][3] = 0;
}
void CGame::AddScene(shared_ptr<CScene> scene)
{
	m_Scenes.push_back(scene);
}
int CGame::SetCurrentScene(int i)
{
	for (unsigned int x = 0 ; x < m_Scenes.size() ; x++ )
	{
		if (x == i)
		{
			m_CurrScene = m_Scenes[x].get();
			return 1;
		}
	}
	return -1;
}
