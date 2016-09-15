#include "GLgame.h"

void CGame::initialize()
{

	displayManager.initialize(1000,600);
	createProjectionMatrix();
    entityRenderer.initialize(projectionMatrix);
	terrainRenderer.init(projectionMatrix);
	guiRenderer.init(1000, 600);
}
void CGame::uninitialize()
{
	if (currScene != nullptr) {
		currScene->cleanUp();
	}
	guiRenderer.cleanUP();
	entityRenderer.uninitialize();
	terrainRenderer.cleanUp();
	displayManager.uninitialize();
}
void CGame::gameLoop()
{
	Uint32 start;
	SDL_Event event;
	bool running = true;
	//renderStartSeries();
    loadScene();

    string loadingText = "FPS : ";
	vector<CGUIText> texts;
	texts.push_back(CGUIText(loadingText, glm::vec2(-0.95,0.9), 1.5, glm::vec3(0, 0, 1)));

	while (running)
	{
		start = SDL_GetTicks();
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(backgroundColour.x, backgroundColour.y, backgroundColour.z, 1);
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
					case SDLK_F9: displayManager.setFullScreen(); break;
					case SDLK_ESCAPE: running = false; break;
					}
					break;
				case SDL_FINGERDOWN:
				{
					cout << "Touch " << endl;
				}
			}
		}

		if (currScene != nullptr) {
			switch (currScene->update(event, displayManager.getWindow()))
			{
			case 1: running = false; break;
			case 2: currScene->cleanUp();  setCurrentScene(1); loadScene();  break;
				
			}
			terrainRenderer.render(currScene,glm::mat4(0));
			entityRenderer.render(currScene);
			guiRenderer.render(currScene->gui);
		}
		texts[0].text = "FPS : " + std::to_string(displayManager.getFps() ) ;
		guiRenderer.renderText(texts);
		
		displayManager.update();

        //cout << displayManager.getFps() << endl ;
		if (1000.0 / displayManager.getFPSCap()>SDL_GetTicks() - start)  SDL_Delay(1000.0 / displayManager.getFPSCap() - (SDL_GetTicks() - start));
	}


}
void CGame::loadScene()
{

	if (currScene == nullptr) return;

	loading = true;
	thread loadingThread(&CGame::initializeScene,this) ;

	currScene->initialize();
//	std::this_thread::sleep_for(10s);

	cout << "Load done." << endl;
	loading = false;

    loadingThread.join();

}
float CGame::fade(Uint32 deltaTime)
{
	bool change = false;
	float alpha = 0.0;
	if (deltaTime < 500) change = true;
	if (deltaTime < 2000 && deltaTime > 500) {

		alpha = (static_cast<float>(deltaTime) - 500.0f) / 2000.0f;
		if (alpha > 1)
			alpha = 1.0;
		change = true;
	}
	if (deltaTime > 4000 ) {
		alpha = ((6000.0f - static_cast<float>(deltaTime)) / 2000.0f);
		//cout << alpha << endl;
		if (alpha < 0)
			alpha = 0;

		change = true;
	}
	if (!change)
		return 1.0;


	return alpha;
}
float CGame::fadeIn(Uint32 deltaTime, Uint32 startTime, Uint32 durration)
{
	float alpha = 0.0;
	alpha = (static_cast<float>(deltaTime) - static_cast<float>(startTime)) / static_cast<float>(durration);
	if (alpha > 1)
		alpha = 1.0;
	return alpha;
}
float CGame::fadeOut(Uint32 deltaTime, Uint32 startTime, Uint32 durration)
{
	float alpha = 0.0;
	alpha = ((static_cast<float>(startTime+ durration) - static_cast<float>(deltaTime)) / static_cast<float>(durration));
	if (alpha < 0)
		alpha = 0;

	return alpha;
}
void CGame::renderStartSeries()
{
	vector<float> vertex = { -0.5,0.5,0,-0.5,-0.5,0,0.5,-0.5,0,0.5,0.5,0 };
	vector<float> textCoords = {
		0,0,
		0,1,
		1,1,
		1,0 };
	vector<unsigned int>indices = { 0,1,3,3,1,2 };

	GLuint VAO = createVAO();
	GLuint ivbo = bindIndicesBuffer(indices);
	GLuint vboId = storeDataInAttributesList(0, 3, vertex);
	GLuint vboTextId = storeDataInAttributesList(1, 2, textCoords);
	unbindVAO();
	loadingShader.init();

	GLuint texture = currScene->loader.loadTexture("Data/GUI/start1.png",true);
	glm::mat4 transformationMatrix = createTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(2.0));


	Uint32 start,start2 = SDL_GetTicks();

	Uint32 deltaTime;
	while (1) {
		deltaTime = SDL_GetTicks() - start2;
		if (deltaTime > 6000) break;
		start = SDL_GetTicks();



		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 1);



		loadingShader.start();

		//loadingShader.loadAlphaValue(fade(deltaTime));
		if (deltaTime > 500 && deltaTime < 2500) {
			loadingShader.loadAlphaValue(fadeIn(deltaTime,500,2000));
		}
		if (deltaTime > 4000) {
			loadingShader.loadAlphaValue(fadeOut(deltaTime, 4000, 2000));
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		loadingShader.loadTransformMatrix(transformationMatrix);
		glBindVertexArray(VAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
		loadingShader.stop();
		displayManager.update();

		if (1000.0 / displayManager.getFPSCap()>SDL_GetTicks() - start)  SDL_Delay(1000.0 / displayManager.getFPSCap() - (SDL_GetTicks() - start));
	}

	loadingShader.stop();
	loadingShader.cleanUp();
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &ivbo);
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &vboTextId);
	glDeleteVertexArrays(1, &VAO);
//	system("pause");
}
void CGame::initializeScene()
{
	//std::this_thread::sleep_for(std::chrono::seconds(30));
	//SDL_GL_MakeCurrent(displayManager.getWindow(), displayManager.glContext);
	SDL_GLContext glLoadingContext = SDL_GL_CreateContext(displayManager.getWindow());
	CGUIRenderer grenderer;
	grenderer.init(1000, 600);


	SDL_Event event;
	float green = 1.0;
	float value = 0.01;

	vector<float> vertex = { -0.5,0.5,0,-0.5,-0.5,0,0.5,-0.5,0,0.5,0.5,0 };
	vector<float> textCoords = {
		0,0,
		0,1,
		1,1,
		1,0};
	vector<unsigned int>indices = { 0,1,3,3,1,2 };

	GLuint VAO = createVAO();
	GLuint ivbo = bindIndicesBuffer(indices);
	GLuint vboId = storeDataInAttributesList(0, 3, vertex);
	GLuint vboTextId = storeDataInAttributesList(1, 2, textCoords);
	unbindVAO();
	loadingShader.init();

	GLuint texture = currScene->loader.loadTexture("Data/GUI/circle2.png");
	currScene->loader.textures.clear();
	glm::mat4 transformationMatrix = createTransformationMatrix(glm::vec3(0), glm::vec3(0), glm::vec3(0.25));

	string loadingText = "Loading";
	vector<CGUIText> texts;
	texts.push_back(CGUIText(loadingText, glm::vec2(-0.15,-0.25), 2, glm::vec3(1, 1, 1)));

	Uint32 xx = 0;

	Uint32 start, start2 = SDL_GetTicks();

	Uint32 deltaTime;

	int numPtr = 0;

	bool endingFade = false;
	while (1)
	{
		start = SDL_GetTicks();
		deltaTime = SDL_GetTicks() - start2;


		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//	glClearColor(backgroundColour.x, green, backgroundColour.z, 1);
		glClearColor(0,0,0, 1);

		loadingShader.start();

		if (deltaTime > 0 && deltaTime < 2000) {
			loadingShader.loadAlphaValue(fadeIn(deltaTime, 0, 1500));
		}

		if (!loading) {
			loading = true;
			endingFade = true;
			start2 = SDL_GetTicks();
			deltaTime = SDL_GetTicks() - start2;
		}
		if (endingFade) {
			if (deltaTime > 2000) break;
		//	cout << fadeOut(deltaTime, 0, 2000) << endl;
			loadingShader.loadAlphaValue(fadeOut(deltaTime, 0, 2000));
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
	//	loadingShader.loadAlphaValue(1.0);
		transformationMatrix *= glm::rotate(-1.0f, 0.0f, 0.0f, 1.0f);
		loadingShader.loadTransformMatrix(transformationMatrix);
		glBindVertexArray(VAO);
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
			case SDL_QUIT: loading = false; break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_F9: displayManager.setFullScreen(); break;
				case SDLK_ESCAPE: loading = false; break;
				}
				break;
			}
		}
		/*green += value;
		if (green > 1.0 || green < 0) value = -value;*/
		//grenderer.render(currScene->getGUI().guiTextures);
		grenderer.renderText(texts);
		displayManager.update();


		if ( start - xx > 500) {
			xx = start;
			numPtr++;
			loadingText += ".";
			if (numPtr > 3) {
				loadingText = "Loading";
				numPtr = 0;
			}
			texts[0].updateText(loadingText);

		}


		if (1000.0 / displayManager.getFPSCap()>SDL_GetTicks() - start)  SDL_Delay(1000.0 / displayManager.getFPSCap() - (SDL_GetTicks() - start));
	}
	loadingShader.stop();
	loadingShader.cleanUp();
	glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &ivbo);
	glDeleteBuffers(1, &vboId);
	glDeleteBuffers(1, &vboTextId);
	glDeleteVertexArrays(1, &VAO);

	grenderer.cleanUP();
	SDL_GL_DeleteContext(glLoadingContext);

}


void CGame::createProjectionMatrix() {

	glm::vec2 windowSize = displayManager.getWindowSize();
	float aspectRatio = (float)windowSize.x / (float)windowSize.y;
	float y_scale = (float)((1.0f / tan(toRadians(FOV / 2.0f))));
	float x_scale = y_scale / aspectRatio;
	float frustum_length = FAR_PLANE - NEAR_PLANE;//	projectionMatrix = glm::mat4(1.0);	return;

	projectionMatrix[0][0] = x_scale;
	projectionMatrix[1][1] = y_scale;
	projectionMatrix[2][2] = -((FAR_PLANE + NEAR_PLANE) / frustum_length);
	projectionMatrix[2][3] = -1;
	projectionMatrix[3][2] = -((2 * NEAR_PLANE * FAR_PLANE) / frustum_length);
	projectionMatrix[3][3] = 0;
}
void CGame::addScene(shared_ptr<CScene> scene)
{
	scenes.push_back(scene);
}
int CGame::setCurrentScene(int i)
{
	for (int x = 0 ; x < scenes.size() ; x++ )	{
		if (x == i) {
			currScene = scenes[x];
	//		cout << "Scene name: " << scenes[x]->getName() << endl;
			return 1;
		}
	}
	return -1;
}
