#include "GLgame.h"

CGame::CGame()
: m_BackgroundColour(0.6, 0.6, 0.8)
, m_WindowSize(1000, 600)
{
}

void CGame::Initialize()
{
	m_DisplayManager.Initialize(static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));
	CreateProjectionMatrix();
    m_EntityRenderer.Initialize(m_ProjectionMatrix);
	m_TerrainRenderer.Init(m_ProjectionMatrix);
	m_GuiRenderer.Init(static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));
}
void CGame::Uninitialize()
{
	if (m_CurrScene != nullptr)
		m_CurrScene->CleanUp();
	
	m_GuiRenderer.CleanUP();
	m_EntityRenderer.Uninitialize();
	m_TerrainRenderer.CleanUp();
	m_DisplayManager.Uninitialize();
}
void CGame::GameLoop()
{
	Uint32 start;
	SDL_Event event;
	bool running = true;
	//renderStartSeries();
    LoadScene();

    string loading_text = "FPS : ";
	vector<CGUIText> texts;
	texts.push_back(CGUIText(loading_text, glm::vec2(-0.95,0.9), 1.5, glm::vec3(0, 0, 1)));

	while (running)
	{
		start = SDL_GetTicks();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(m_BackgroundColour.x, m_BackgroundColour.y, m_BackgroundColour.z, 1);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT: running = false; break;
				case SDL_MOUSEBUTTONDOWN:

					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_F9: m_DisplayManager.SetFullScreen(); break;
					case SDLK_ESCAPE: running = false; break;
					}
					break;
				case SDL_FINGERDOWN:
				{
					cout << "Touch " << endl;
				}
			}
		}

		if (m_CurrScene != nullptr)
		{
			switch (m_CurrScene->Update(event, m_DisplayManager.GetWindow()))
			{
			case 1: running = false; break;
			case 2: m_CurrScene->CleanUp();  SetCurrentScene(1); LoadScene();  break;				
			}
			m_TerrainRenderer.Render(m_CurrScene,glm::mat4(0));
			m_EntityRenderer.Render(m_CurrScene);
			m_GuiRenderer.Render(m_CurrScene->GetGui());
		}
		texts[0].updateText("FPS : " + std::to_string(m_DisplayManager.GetFps() ) );
		m_GuiRenderer.RenderText(texts);
		
		m_DisplayManager.Update();

		if (static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) > SDL_GetTicks() - start)  SDL_Delay(static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) - (SDL_GetTicks() - start));
	}
}
void CGame::LoadScene()
{
	if (m_CurrScene == nullptr) return;

	m_IsLoading = true;
	thread loading_thread(&CGame::InitializeScene,this) ;

	m_CurrScene->Initialize();

	m_IsLoading = false;

	loading_thread.join();
}
float CGame::Fade(Uint32 delta_time)
{
	bool change = false;
	float alpha = 0.0;
	if (delta_time < 500) change = true;
	if (delta_time < 2000 && delta_time > 500) {

		alpha = (static_cast<float>(delta_time) - 500.0f) / 2000.0f;
		if (alpha > 1)
			alpha = 1.0;
		change = true;
	}
	if (delta_time > 4000 ) {
		alpha = ((6000.0f - static_cast<float>(delta_time)) / 2000.0f);
		if (alpha < 0)
			alpha = 0;
		change = true;
	}
	if (!change)
		return 1.0;

	return alpha;
}
float CGame::FadeIn(Uint32 delta_time, Uint32 start_time, Uint32 durration)
{
	float alpha = 0.0;
	alpha = (static_cast<float>(delta_time) - static_cast<float>(start_time)) / static_cast<float>(durration);
	if (alpha > 1)
		alpha = 1.0;
	return alpha;
}
float CGame::FadeOut(Uint32 delta_time, Uint32 start_time, Uint32 durration)
{
	float alpha = 0.0;
	alpha = ((static_cast<float>(start_time + durration) - static_cast<float>(delta_time)) / static_cast<float>(durration));
	if (alpha < 0)
		alpha = 0;

	return alpha;
}
void CGame::RenderStartSeries()
{
	vector<float> vertex = { -0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0 };
	vector<float> text_coords = {
		0, 0,
		0, 1,
		1, 1,
		1, 0 };
	vector<unsigned int>indices = { 0, 1, 3, 3, 1, 2 };

	GLuint vao		   = Utils::CreateVao();
	GLuint i_vbo	   = Utils::BindIndicesBuffer(indices);
	GLuint vbo_id	   = Utils::StoreDataInAttributesList(0, 3, vertex);
	GLuint vbo_text_id = Utils::StoreDataInAttributesList(1, 2, text_coords);
	Utils::UnbindVao();
	m_LoadingShader.Init();

	GLuint texture = m_CurrScene->GetLoader().LoadTexture("Data/GUI/start1.png",true);
	glm::mat4 transformation_matrix = Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(2.0));

	Uint32 start,start2 = SDL_GetTicks();

	Uint32 delta_time;
	while (1) 
	{
		delta_time = SDL_GetTicks() - start2;
		if (delta_time > 6000) break;
		start = SDL_GetTicks();

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);

		m_LoadingShader.Start();

		if (delta_time > 500 && delta_time < 2500) {
			m_LoadingShader.LoadAlphaValue(FadeIn(delta_time, 500, 2000));
		}
		if (delta_time > 4000) {
			m_LoadingShader.LoadAlphaValue(FadeOut(delta_time, 4000, 2000));
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		m_LoadingShader.LoadTransformMatrix(transformation_matrix);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		m_LoadingShader.Stop();
		m_DisplayManager.Update();

		if ( static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) > SDL_GetTicks() - start)  SDL_Delay(static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) - (SDL_GetTicks() - start));
	}
	m_LoadingShader.Stop();
	m_LoadingShader.CleanUp();
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &i_vbo);
	glDeleteBuffers(1, &vbo_id);
	glDeleteBuffers(1, &vbo_text_id);
	glDeleteVertexArrays(1, &vao);
}
void CGame::InitializeScene()
{
	SDL_GLContext gl_loading_context = SDL_GL_CreateContext(m_DisplayManager.GetWindow());
	CGUIRenderer grenderer;
	grenderer.Init(static_cast<int>(m_WindowSize.x), static_cast<int>(m_WindowSize.y));

	SDL_Event event;
	float green = 1.0f;
	float value = 0.01f;

	vector<float> vertex = { -0.5, 0.5, 0, -0.5, -0.5, 0, 0.5, -0.5, 0, 0.5, 0.5, 0};
	vector<float> text_coords = {
		0, 0,
		0, 1,
		1, 1,
		1, 0};
	vector<unsigned int> indices = {0, 1, 3, 3, 1, 2};

	GLuint vao			= Utils::CreateVao();
	GLuint i_vbo		= Utils::BindIndicesBuffer(indices);
	GLuint vbo_id		= Utils::StoreDataInAttributesList(0, 3, vertex);
	GLuint vbo_text_id  = Utils::StoreDataInAttributesList(1, 2, text_coords);
	Utils::UnbindVao();
	m_LoadingShader.Init();

	GLuint texture = m_CurrScene->GetLoader().LoadTexture("Data/GUI/circle2.png");
	m_CurrScene->GetLoader().m_Textures.clear();
	glm::mat4 transformation_matrix = Utils::CreateTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(0.25));

	string loading_text = "Loading";
	vector<CGUIText> texts;
	texts.push_back(CGUIText(loading_text, glm::vec2(-0.15,-0.25), 2, glm::vec3(1, 1, 1)));

	Uint32 xx = 0;

	Uint32 start, start2 = SDL_GetTicks();

	Uint32 delta_time;

	int numPtr = 0;

	bool ending_fade = false;
	while (1)
	{
		start = SDL_GetTicks();
		delta_time = SDL_GetTicks() - start2;

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glClearColor(backgroundColour.x, green, backgroundColour.z, 1);
		glClearColor(0, 0, 0, 1);

		m_LoadingShader.Start();

		if (delta_time > 0 && delta_time < 2000)
		{
			m_LoadingShader.LoadAlphaValue(FadeIn(delta_time, 0, 1500));
		}

		if (!m_IsLoading)
		{
			m_IsLoading = true;
			ending_fade = true;
			start2 = SDL_GetTicks();
			delta_time = SDL_GetTicks() - start2;
		}
		if (ending_fade)
		{
			if (delta_time > 2000) break;
			m_LoadingShader.LoadAlphaValue(FadeOut(delta_time, 0, 2000));
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		transformation_matrix *= glm::rotate(-1.0f, 0.0f, 0.0f, 1.0f);
		m_LoadingShader.LoadTransformMatrix(transformation_matrix);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: m_IsLoading = false; break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_F9: m_DisplayManager.SetFullScreen(); break;
				case SDLK_ESCAPE: m_IsLoading = false; break;
				}
				break;
			}
		}
		/*green += value;
		if (green > 1.0 || green < 0) value = -value;*/
		//grenderer.render(currScene->getGUI().guiTextures);
		grenderer.RenderText(texts);
		m_DisplayManager.Update();

		if ( start - xx > 500) 
		{
			xx = start;
			numPtr++;
			loading_text += ".";
			if (numPtr > 3) {
				loading_text = "Loading";
				numPtr = 0;
			}
			texts[0].updateText(loading_text);
		}


		if (static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) > SDL_GetTicks() - start)  SDL_Delay(static_cast<Uint32>(1000.0f / m_DisplayManager.GetFPSCap()) - (SDL_GetTicks() - start));
	}
	m_LoadingShader.Stop();
	m_LoadingShader.CleanUp();
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &i_vbo);
	glDeleteBuffers(1, &vbo_id);
	glDeleteBuffers(1, &vbo_text_id);
	glDeleteVertexArrays(1, &vao);

	grenderer.CleanUP();
	SDL_GL_DeleteContext(gl_loading_context);
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
			m_CurrScene = m_Scenes[x];
			return 1;
		}
	}
	return -1;
}
