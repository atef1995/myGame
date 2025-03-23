#ifndef APPLICATION_H
#define APPLICATION_H

#include <SDL3/SDL.h>
#include <string>

class Application {
public:
	Application(const std::string& title, int width, int height);
	~Application();

	void Run();
private:
	bool InitSDL(const std::string& title, int width, int height);
	void ShutdownSDL();
	void ProcessInput();
	void Update(float deltaTime);
	void Render();

	SDL_Window* window;
	SDL_GLContext glContext;
	bool isRunning;
	int windowWidth;
	int windowHeight;
};


#endif