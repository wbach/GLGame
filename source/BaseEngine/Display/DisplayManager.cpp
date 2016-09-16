#include "DisplayManager.h"

int CDisplayManager::initialize(int w, int h)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	unsigned int flags = SDL_WINDOW_OPENGL;
	if (!(window = SDL_CreateWindow("My OpenGL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, flags))) {
		cout << "failed to create window\n";
		exit(-1);
	}
	if (!(m_GlContext = SDL_GL_CreateContext(window))) {
		cout << "failed to create OpenGL context\n";
		exit(-1);
	}

	GLint GlewInitResult = glewInit();
	if (GlewInitResult != GLEW_OK) {
		printf("ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(-1);
	}
	printf("GL version: %s\n\n", glGetString(GL_VERSION));

	windowsSize.x = w;
	windowsSize.y = h;

	FPS_CAP = 120;

	return 0;
}

void CDisplayManager::update()
{
    calculateFPS();
	float currentFrameTime = getCurrentTime();
	delta = (currentFrameTime - lastFrameTime) / 1000.0f;
	SDL_GL_SwapWindow(window);
}
float CDisplayManager::getCurrentTime() {
	return SDL_GetTicks();
}
void CDisplayManager::uninitialize()
{
	SDL_GL_DeleteContext(m_GlContext);
	SDL_Quit();
}
void CDisplayManager::calculateFPS()
{
	frameCount++;

	currentTime = SDL_GetTicks();

	int timeInterval = currentTime - previousTime;

	if(timeInterval > 1000)
	{
		fps = frameCount / (timeInterval / 1000.0f);
		previousTime = currentTime;
		frameCount = 0;
	}
}
int CDisplayManager::getFps()
{
	return (int)fps;
}
void CDisplayManager::setFullScreen()
{
	fullScreen = !fullScreen;
	if (fullScreen) {
		SDL_SetWindowFullscreen(window, SDL_TRUE);
	}else
		SDL_SetWindowFullscreen(window, SDL_FALSE);
}

const glm::vec2& CDisplayManager::getWindowSize() {
	return windowsSize;
}
