#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "ModuleFadeToBlack.h"
#include "ModuleMenu.h"
#include "ModulePlayer.h"
#include "ModuleFont.h"

ModuleMenu::ModuleMenu(bool active) : Module(active)
{
	robot.frames.push_back({ 2,0,280,325 });
}

ModuleMenu::~ModuleMenu()
{}

// Load assets
bool ModuleMenu::Start()
{
	LOG("Loading space intro");
	
	background = App->textures->Load("assets/intro.png");

	if(fx == 0)
		fx = App->audio->LoadFx("assets/music/SFX/Coin.wav");

	return true;
}

// UnLoad assets
bool ModuleMenu::CleanUp()
{
	LOG("Unloading space scene");

	App->textures->Unload(background);
	
	return true;
}

// Update: draw background
update_status ModuleMenu::Update()
{
	App->renderer->Blit(background, 0, 0, nullptr, nullptr );


	if(App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (App->fade->isFading() == false) {
			App->player->SetLives(3);
			App->fade->FadeToBlack((Module*)App->scene, this);
			App->audio->PlayFx(fx);
		}
	}

	return UPDATE_CONTINUE;
}