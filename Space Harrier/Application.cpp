#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleFadeToBlack.h"
#include "ModuleCollision.h"
#include "ModuleParticles.h"
#include "ModuleEnemy.h"
#include "ModuleFont.h"
#include "ModuleFloor.h"

#include "ModuleSceneIntro.h"
#include "ModuleScene.h"
#include "ModulePlayer.h"

using namespace std;

Application::Application()
{
	// Order matters: they will init/start/pre/update/post in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());

	modules.push_back(textures = new ModuleTextures());
	modules.push_back(audio = new ModuleAudio());
	modules.push_back(floor = new ModuleFloor);

	// Game Modules
	modules.push_back(scene_intro = new ModuleSceneIntro(false));
	modules.push_back(player = new ModulePlayer(false));
	modules.push_back(scene_space = new ModuleScene(false));

	// Modules to draw on top of game logic
	modules.push_back(collision = new ModuleCollision());
	modules.push_back(particles = new ModuleParticles());
	modules.push_back(fonts = new ModuleFont());
	modules.push_back(enemies = new ModuleEnemy());

	//Renderer must be here to draw from buffer after all other modules had request to blit
	modules.push_back(renderer = new ModuleRender());

	//Fade to black is the last one, in order to work properly
	modules.push_back(fade = new ModuleFadeToBlack());

	playing = false;
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
		RELEASE(*it);
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init(); // we init everything, even if not anabled

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
	{
		if((*it)->IsEnabled() == true)
			ret = (*it)->Start();
	}

	// Start the first scene --
	fade->FadeToBlack(scene_intro, nullptr, 3.0f);

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PreUpdate();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->Update();

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->PostUpdate();

	//Game pause
	if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && playing) {
		//Hacer copia del renderer actual
		while (true)
		{
			//Ir printeando a intervalos "PAUSE" en el renderer, alternando entre la copia, 
			//y otra surface con el string pintado
			if (input->PreUpdate() == UPDATE_STOP) {
				return UPDATE_STOP;
			 }
			if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) {
				break;
			}
		}
		//al salir, volver a colocar la copia al renderer, para que siga como antes
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		if((*it)->IsEnabled() == true) 
			ret = (*it)->CleanUp();

	return ret;
}

