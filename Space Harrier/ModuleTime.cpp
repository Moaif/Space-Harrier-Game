#include "ModuleTime.h"
#include "SDL\include\SDL.h"


ModuleTime::ModuleTime() {
	previousTicks = 0;
	deltaTime = 0;
}

ModuleTime::~ModuleTime() {

}

update_status ModuleTime::PreUpdate() {
	float currentTicks = (float)SDL_GetTicks();
	deltaTime = (currentTicks - previousTicks)/1000.0f;
	previousTicks = currentTicks;

	return UPDATE_CONTINUE;
}

float ModuleTime::GetDeltaTime() {
	return deltaTime;
}