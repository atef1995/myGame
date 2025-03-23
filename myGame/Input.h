#ifndef INPUT_H
#define INPUT_H

#include <SDL3/SDL.h>
#include <unordered_map>

enum class KeyState {
	Idle,
	Pressed,
	Held,
	Released
};

class Input {
public:
	static Input& Get();

	void ProcessEvent(const SDL_Event& event);
	void Update();

	// Key queries
	bool IsKeyPressed(SDL_Scancode key);
	bool IsKeyHeld(SDL_Scancode key);
	bool IsKeyReleased(SDL_Scancode key);

	// Mouse queries
	bool IsMouseButtonPressed(Uint8 button);
	bool IsMouseButtonHeld(Uint8 button);
	bool IsMouseButtonReleased(Uint8 button);
	int GetMouseX() const;
	int GetMouseY() const;

private:
	Input();
	std::unordered_map<SDL_Scancode, KeyState> keyStates;
	std::unordered_map<Uint8, KeyState> mouseButtonStates;

	int mouseX, mouseY;
};

#endif // !INPUT_H
