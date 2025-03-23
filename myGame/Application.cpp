#include "Application.h"
#include <iostream>
#include <glad/glad.h>
#include "Input.h"

Application::Application(const std::string& title, int width, int height)
	:window(nullptr), glContext(nullptr), isRunning(false), windowWidth(width), windowHeight(height)
{
	if (InitSDL(title,width,height))
	{
		isRunning = true;
	}
}

Application::~Application() {
	ShutdownSDL();
}

bool Application::InitSDL(const std::string& title, int width, int height) {
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMEPAD|SDL_INIT_AUDIO) != 0)
	{
		std::cerr << "SDL Initialization failed: " << SDL_GetError() << std::endl;
		return false;
	}

	window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL);
	if (!window)
	{
		std::cerr << "window creation failed: " << SDL_GetError() << std::endl;
		return false;
	}

	glContext = SDL_GL_CreateContext(window);
	if (!glContext)
	{
		std::cerr << "OpenGL context creation failed: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_GL_MakeCurrent(window, glContext);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetSwapInterval(1);

	return true;
}

void Application::ShutdownSDL() {
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Application::Run() {
	float deltaTime = 0.0f;
	Uint64 currentTime = SDL_GetTicks();

	while (isRunning)
	{
		Uint64 newTime = SDL_GetTicks();
		deltaTime = (newTime - currentTime) / 1000.0f;
		currentTime = newTime;
		ProcessInput();
		Update(deltaTime);
		Render();
		SDL_GL_SwapWindow(window);
	}
}

void Application::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			isRunning = false;
		}

		Input::Get().ProcessEvent(event);
	}

	Input::Get().Update(); // call once per frame
}

void Application::Update(float deltaTime) {
	// for physics updates etc
}

void Application::Render() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}