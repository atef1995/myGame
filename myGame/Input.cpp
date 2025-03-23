#include "input.h"

Input& Input::Get() {
	static Input instance;
	return instance;
}

Input::Input():mouseX(0),mouseY(0){}

void Input::ProcessEvent(const SDL_Event& event) {
	switch (event.type) {
	case SDL_EVENT_KEY_DOWN: {
		SDL_Scancode key = event.key.scancode;
		if (keyStates[key] != KeyState::Held)
		{
			keyStates[key] = KeyState::Pressed;
		}
		break;
	}
	case SDL_EVENT_KEY_UP: {
		SDL_Scancode key = event.key.scancode;
		keyStates[key] = KeyState::Released;
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_DOWN:{
		Uint8 button = event.button.button;
		if (mouseButtonStates[button] != KeyState::Held)
		{
			mouseButtonStates[button] = KeyState::Pressed;
		}
		break;
	}
	case SDL_EVENT_MOUSE_BUTTON_UP: {
		Uint8 button = event.button.button;
		mouseButtonStates[button] = KeyState::Released;
		break;
	}
	case SDL_EVENT_MOUSE_MOTION: {
		mouseX = event.motion.x;
		mouseY = event.motion.y;
		break;
	}
	}
}

void Input::Update() {
	for (auto& [key,state]:keyStates)
	{
		if (state == KeyState::Pressed) state = KeyState::Held;
		else if (state == KeyState::Released) state = KeyState::Released;
	}

	for (auto& [button,state] : mouseButtonStates)
	{
		if (state == KeyState::Pressed) state = KeyState::Held;
		else if (state == KeyState::Released) state = KeyState::Released;
	}
}

bool Input::IsKeyPressed(SDL_Scancode key) {
	return keyStates[key] == KeyState::Pressed;
}

bool Input::IsKeyHeld(SDL_Scancode key) {
	return keyStates[key] == KeyState::Held;
}

bool Input::IsKeyReleased(SDL_Scancode key) {
	return keyStates[key] == KeyState::Released;
}

// --- Mouse Queries ---
bool Input::IsMouseButtonPressed(Uint8 button) {
	return mouseButtonStates[button] == KeyState::Pressed;
}

bool Input::IsMouseButtonHeld(Uint8 button) {
	return mouseButtonStates[button] == KeyState::Held;
}

bool Input::IsMouseButtonReleased(Uint8 button) {
	return mouseButtonStates[button] == KeyState::Released;
}

int Input::GetMouseX() const {
	return mouseX;
}

int Input::GetMouseY() const {
	return mouseY;
}